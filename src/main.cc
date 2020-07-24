#include <global.h>
#include <checkArgs.hpp>

uint64_t* ArregloComun = nullptr;
uint64_t* ArregloSerial = nullptr;
uint64_t* ArregloParalelo = nullptr;
uint64_t* ArregloOpenMPSerial= nullptr;
uint64_t* ArregloOpenMPParalelo = nullptr;
uint64_t SumaOpenParalelo= 0;
uint64_t SumaParalelo = 0;
std::vector<std::thread *> threads;
std::vector<std::thread *> threadsSumas;
std::mutex Candado;

void LLenadoArreglo(size_t Left, size_t Right, size_t RandType, uint64_t numThreads){
	for(size_t i = Left; i < Right; ++i){		
		switch(RandType){
			case 0: 
				ArregloSerial[i] = ArregloComun[i]; 
				break;
			case 1:
				ArregloParalelo[i] = ArregloComun[i];
				break;
		}
	}
}
void SumadoParalelo(uint64_t Left, uint64_t Right){
	Candado.lock();
	for(uint64_t i = Left; i < Right; i++){
		SumaParalelo += ArregloParalelo[i];
	}
	Candado.unlock();
}

int main(int argc, char** argv){
	
	uint64_t totalElementos;
	uint64_t numThreads;
	uint64_t LInferior;
	uint64_t LSuperior;
	
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	
	totalElementos = argumentos->getArgs().TamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	LInferior	   = argumentos->getArgs().LInferior;
	LSuperior 	   = argumentos->getArgs().LSuperior;	
	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads     << std::endl;
	std::cout << "Limite Inferior Rand: "<< LInferior << std::endl;
	std::cout << "Limite Superior Rand: "<< LSuperior << std::endl;
	
	//================Modulo LLenado================
	ArregloComun = new uint64_t[totalElementos];
	std::srand(std::time(0)); 
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<uint32_t> unif(LInferior, LSuperior);
	for(uint64_t i = 0; i < totalElementos; i++){
		ArregloComun[i] = unif(rng);	
	}

	//======Llenado En Serie======
	ArregloSerial = new uint64_t[totalElementos];
	auto start = std::chrono::high_resolution_clock::now();
	LLenadoArreglo(0,totalElementos,0,numThreads);
	auto end     = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeS = elapsed.count();


	//======Llenado En Paralelo======
	ArregloParalelo = new uint64_t[totalElementos];
	for (size_t i= 0; i < numThreads ; i++){
		threads.push_back(new std::thread(LLenadoArreglo, i*(totalElementos)/numThreads, (i+1)*(totalElementos)/numThreads,1, numThreads));
	}
	start = std::chrono::high_resolution_clock::now();
	for(auto &thLlenado : threads){
		thLlenado->join();
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeP = elapsed.count();
	

	//================Modulo Sumado================

	//======Sumado En Serie======
	uint64_t SumaSerial=0;
	start     = std::chrono::high_resolution_clock::now(); 
	for (size_t i=0;i<totalElementos;i++){
		SumaSerial += ArregloSerial[i];
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSumaS = elapsed.count();	

	//======Sumado En Paralelo======
	for (size_t i=0;i<numThreads;i++){
		threadsSumas.push_back(new std::thread(SumadoParalelo, i*(totalElementos)/numThreads,(i+1)*(totalElementos)/numThreads));
	}
	start     = std::chrono::high_resolution_clock::now();
	for(auto &thS : threadsSumas){
		thS->join();
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSumaP = elapsed.count();

	//======Llenado en Serie con openMP======
	ArregloOpenMPSerial = new uint64_t[totalElementos];
	start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for num_threads(1)
	{
	for(size_t i=0; i<totalElementos; i++){
		ArregloOpenMPSerial[i] = ArregloComun[i];
	}
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeOMPSerial = elapsed.count();
		
	//======Llenado en paralelo con openMP======
	ArregloOpenMPParalelo = new uint64_t[totalElementos];
	start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for num_threads(numThreads)
	{
	for(size_t i=0; i<totalElementos; i++){
		ArregloOpenMPParalelo[i] = ArregloComun[i];
	}
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeOMPParalelo = elapsed.count();

	//======Sumado en Serial con OpenMP======
	uint64_t sumaSerialOMP = 0;
	start     = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for reduction(+:sumaSerialOMP) num_threads(1)
	{
	for(size_t i = 0; i < totalElementos ; ++i){
		sumaSerialOMP += ArregloOpenMPParalelo[i];
	}
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSSumaOMP = elapsed.count();
	//======Sumado en Paralelo con OpenMP======
	uint64_t sumaParaleloOMP = 0;
	start     = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for reduction(+:sumaParaleloOMP) num_threads(numThreads)
	{
	for(size_t i = 0; i < totalElementos ; ++i){
		sumaParaleloOMP += ArregloOpenMPParalelo[i];
	}
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimePSumaOMP = elapsed.count();
	
	//================Consolidacion de resultados================
	std::cout << "Suma En Serie:                     " <<SumaSerial << std::endl;
	std::cout << "Suma Total en Paralelo:            " << SumaParalelo << std::endl;
	std::cout << "Suma Total en Serie con OMP:       " << sumaSerialOMP << std::endl;
	std::cout << "Suma Total en Paralelo con OMP:    " << sumaParaleloOMP << "\n" << std::endl;
	std::cout << "==========Tiempos de Llenado==========" << std::endl;
	std::cout << "TiempoSerial :            "  << totalTimeS << "[ms]" << std::endl;
	std::cout << "TiempoParalelo:           "  << totalTimeP << "[ms]" << std::endl;
	std::cout << "TiempoSerial con OMP:     "  << totalTimeOMPSerial << "[ms]" << std::endl;
	std::cout << "TiempoParalelo con OMP:   "  << totalTimeOMPParalelo << "[ms]" <<"\n"<< std::endl;
	std::cout << "==========Tiempos de Sumado==========" << std::endl;
	std::cout << "TiempoSerial:             "  << totalTimeSumaS << "[ms]" << std::endl;
	std::cout << "TiempoParalelo:           "  << totalTimeSumaP << "[ms]" << std::endl;
	std::cout << "TiempoSerial con OMP:     " << totalTimeSSumaOMP << "[ms]" << std::endl;
	std::cout << "TiempoParalelo con OMP:   " << totalTimePSumaOMP << "[ms]" << "\n" << std::endl;

	std::cout << "==========Resumen de Desempeños==========" << std::endl;
	std::cout << "======Desempeño Llenado sin OMP======"<< std::endl;
	std::cout << "Llenado Entre Serial y Paralelo =        " << (double)totalTimeS/totalTimeP <<std::endl;
	std::cout << "======Desempeño Sumado sin OMP======"<< std::endl;
	std::cout << "Sumado Entre Serial y Paralelo =        " << (double)totalTimeSumaS/totalTimeSumaP << "\n" <<std::endl;

	std::cout << "======Desempeño con OMP======"<< std::endl;
	std::cout << "OMP LLenado Entre Serial y Paralelo =    " << (double)totalTimeOMPSerial/totalTimeOMPParalelo <<std::endl;
	std::cout << "OMP Sumado Entre Serial y Paralelo =     " << (double)totalTimeSSumaOMP/totalTimePSumaOMP << "\n" <<std::endl;

	std::cout << "======Desempeño de metodos======"<< std::endl;
	std::cout << "Llenado Solo Serial y OMP Serial =         " << (double)totalTimeS/totalTimeOMPSerial <<std::endl;
	std::cout << "Sumado Solo Serial y OMP Serial =          " << (double)totalTimeSumaS/totalTimeSSumaOMP <<std::endl;
	std::cout << "Llenado Solo Paralelo y OMP Paralelo =     " << (double)totalTimeP/totalTimeOMPParalelo <<std::endl;
	std::cout << "Sumado Solo Paralelo y OMP Paralelo =      " << (double)totalTimeSumaP/totalTimePSumaOMP <<std::endl;

	return(EXIT_SUCCESS);
}

