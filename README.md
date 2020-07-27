# TSSOO-taller03

##### Fernando Del Pino Machuca - fernando.delpino@alumnos.uv.cl

###### Universidad de Valparaíso

---


## 1. Introducción
 
El siguiente repositorio se basa en el desarrollo de la actividad del taller 3 de la asignatura taller de sistemas operativos. El objetivo y propósito de este, no es más que la utilización y manejo de Threads (Hilos) para almacenar y sumar datos de un arreglo de números aleatorios, mediante el uso de programación paralela a raíz de la implementación de la API de OpenMP.
La resolución de este taller está basado en el lenguaje de programación [C++](https://es.wikipedia.org/wiki/C%2B%2B), junto con el manejo de hilos a través de [OpenMP](https://www.openmp.org) y además, todo el proceso resolutivo y de pruebas se ejecutó en una máquina virtual con sistema operativo Ubuntu-server.
 
## 2. Diseño de Solución
 
Este Taller presenta 2 módulos, un módulo de llenado de arreglo, y otro módulo donde se realizará la suma del arreglo anterior, todo esto se realizará mediante un proceso de rellenado en paralelo mediante OMP(OpenMP), además de implementar el taller anterior (taller 02), para comparar resultados correspondientes a las métricas de desempeño, en los tiempos de ejecución de cada módulo.
 
El orden y la estructura de la solución propuesta se basa a grandes rasgos en el siguiente diseño.
 
![Imgur](https://i.imgur.com/bhb79D8.jpg)
 
Al momento de ingresar los argumentos se debe especificar el tamaño de elementos que poseerá el arreglo, luego se debe incluir la cantidad de hilos que se desean crear para la ejecución del programa, y finalmente se incluirán los límites inferiores y superiores que funcionaran como rango de números aleatorios con los que se rellenaran los arreglos. Tras este proceso se realiza el proceso de creación de los arreglos, el arreglo serial y uno paralelo para la comparación de los resultados de desempeños. Una vez creado el arreglo de números aleatorios serial, se traspasaran los elementos hacia un arreglo que será utilizado por los hilos de forma paralela, para luego entregar un solo arreglo que será dividido nuevamente por otros hilos para lograr la suma de estos elementos. 
 
## 3. Estructura del Código
 
### 3.1 Chequeo de argumentos
 
Para comenzar se debe destacar que el archivo "checkArgs.hpp" se encargará de traspasar los argumentos que se ingresen al programa para ser utilizados como variables por este mismo. Su comportamiento y análisis es simple.
 
### 3.2 Módulo 1
 
Nos centraremos más que nada en la implementación de OMP, debido a que la explicación de la implementación de hilos y en serie ya se realizó en el taller anterior, para crear el arreglo serial y paralelo, donde el arreglo serial se rellenará de la siguiente manera:
```
#pragma omp parallel for  num_threads(1)
    for(size_t i=0; i<totalElementos; i++){
        ArregloOpenMPSerial[i] = ArregloComun[i];
    }
 
```
Lo que se realizará es crear un hilo, para que realice de manera secuencial, el llenado del arreglo serial utilizando OpenMP, a raíz de un arreglo común que posee elementos aleatorios.
 
Paso seguido se rellenará el arreglo de forma paralela:
 
```
#pragma omp parallel for  num_threads(numThreads)
    for(size_t i=0; i<totalElementos; i++){
        ArregloOpenMPParalelo[i] = ArregloComun[i];
    }
```
 
### 3.2 Módulo 2.
 
Este módulo se ideó simplemente para ir sumando a través de una variable, todos los elementos que se encontraban dentro del arreglo paralelo y serial, de la siguiente manera:
 
```
#pragma omp parallel for reduction(+:sumaParaleloOMP) num_threads(numThreads)
    for(size_t i = 0; i < totalElementos ; ++i){
        sumaParaleloOMP += ArregloOpenMPParalelo[i];
    }
```
Para mantener la consistencia de los datos a la hora de realizar el sumado de la variable "SumaParaleloOMP", se debe agregar la cláusula "reduction()" dentro de la implementación de OMP, para evitar inconsistencias a la hora de obtener resultados
 
## 4. Conclusión.
 
Todo este programa está basado en su implementación dentro de una máquina virtual con sistema operativo basado en Linux, es muy posible que ciertas características no sean del todo correctas para otros sistemas operativos y ciertas funcionalidades del programa no sean eficazmente logradas dependiendo de las especificaciones de cada máquina virtual donde se ejecute este programa.
 
En base a los diseños planteados, se comprendió que estos eran lo suficientemente completos como para realizar todo el proceso de codificación, aunque si bien es cierto, el taller anterior jugó un papel importante a la hora de la comparación e implementación de resultados esperados. El proceso de entendimiento de los resultados y análisis de estos fue un tanto complejo, debido a que se presentaron varias trabas a la hora de implementar la solución correcta, con respecto a cómo se deben comportar los hilos en llenado y suma de elementos de un arreglo. Aparte de esto factores como el tiempo, tanto para la toma de resultados, como también los tiempos de desarrollo fueron un tanto extensos, sobre todo por la recopilación de los resultados, debido a las limitaciones que presentaba el procesamiento de las tareas en la máquina virtual. Como síntesis de todo, se realizó un proceso de desarrollo, con respecto a un programa que manejara hilos, mediante una previa implementación (mejorando algunos aspectos en esta entrega) y una implementación que usará OpenMP, para la creación y recopilación de las tareas que realizaba cada Thread, donde se concluye que este proceso se realizó, aunque no de la manera esperada, satisfactoriamente obteniendo gran parte de los resultados esperados con respecto al comportamiento y manejo de hilos, donde el detalle principal viene dado por parte del módulo de sumado, debido a que los tiempos de suma de cada arreglo tomaron mucho más tiempo para la implementación pasada, en contraste con la implementación de OpenMP, dejándonos en claro que este proceso puede ser mucho mejor implementado, para corresponder con los resultados de optimización de tareas esperados por parte del uso de Threads. Para futuros estudios personales se retomará este mismo problema con el afán de resolver las problemáticas referidas a los tiempos de resultados en el módulo de sumado, en vista y propósito de mejorar su implementación dejando de lado el uso de Mutex. Sin más que decir, se concluye este reporte de actividad.
 
Se puede apreciar que el cumplimiento del taller no abarcó por completo ciertos aspectos:
 
```diff
- Optimización de los tiempos de sumado por parte de la implementacion previa con Threads (taller previo)
- Cierto entendimiento de estructuras de vectores para el mejor manejo de los hilos (taller previo)
```
 
Aunque también es cierto que el proceso de aprendizaje fue logrado para la completación de los siguientes aspectos:
 
```diff
+ Implementación del módulo 1
+ Implementación del módulo 2
+ Mejor Entendimiento del funcionamiento de Hilos y su desempeño en comparativa de actividades secuenciales
+ Cierto grado del manejo de tareas por parte de Threads a través de OpenMP
+ Mayor comprensión del funcionamiento y desarrollo de programas en lenguaje C++
+ Entendimiento del comportamiento y utilización de la memoria física y virtual de un equipo computacional
```
 

