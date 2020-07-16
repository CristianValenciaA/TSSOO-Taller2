# TSSOO- Taller 02

Cristian Valencia Avila - cristian.valenciaa@alumnos.uv.cl


# 1. Introducción 
En el desarrollo del siguiente taller, se realiza el llenado de un arreglo dinámico y además la suma respectiva de los valores guardados en el arreglo dinámico creado, para esto se realiza un código implementado en c++ , el cual consta de dos módulos:
1.- Modulo de llenado de arreglo 
2.- Modulo de suma del arreglo
Para realizar pruebas de desempeño para dichos módulos, se realizan dos tipos de procesos de llenado y suma. Estos procesos se implementaran de manera secuencial y paralela, y con esto se calculará su tiempo de ejecución, para tener una visualización del desempeño de cada modulo.
 
# 2. Diseño de la Solución

El taller posee dos módulos principales los cuales son el llenado de un arreglo dinámico y la suma de los elementos del arreglo ambos procesos de manera paralela.
El modelado de la solución de los módulos es el siguiente:

![Diagrama solución](http://imgfz.com/i/pRj1LuH.png)

A continuación se explica cada modulo por separado

### 2.1 Modulo 1 

En primer lugar se crea un arreglo, el cual es llenado con datos de forma aleatoria por medio de la siguiente función:

```
void LlenadoDelArreglo(size_t beginIdx, size_t endIdx, uint32_t limiteInferior, uint32_t limiteSuperior, size_t Rnetype){
std::random_device device;
std::mt19937 rng(device());
std::uniform_int_distribution<uint32_t> nRandom(limiteInferior, limiteSuperior);
for (size_t i = beginIdx; i < endIdx; ++i){
	uint32_t random = nRandom(rng);
	switch (Rnetype){
	case 0:
		arregloEnParalelo[i] = random;
		break;
	case 1:
		arregloEnSerie[i] = random;
		break;
	}
}

```             
Donde a continuación dentro de la función main se implementa la siguiente sección para el llenado del arreglo: 
```
arregloEnParalelo = new uint64_t[totalElementos];
for (size_t i = 0; i < numThreads; ++i){
	threads.push_back(new std::thread(LlenadoDelArreglo,
	i * (totalElementos) / numThreads,
	(i + 1) * (totalElementos) / numThreads, limInf, limSup, 0));
}
```
Donde se encarga de llenar el arreglo de manera paralela, y para medir el tiempo de ejecución del bloque mostrado se utiliza el método de` std::chrono`, el cual nos mostrará el tiempo del llenado:
```
start = std::chrono::system_clock::now();
for (auto &thrs : threads){
	thrs->join();
}
end = std::chrono::system_clock::now();
std::chrono::duration<float, std::milli> duracionLlenado = end - start;
auto tiempoArrayP = duracionLlenado.count();
```

### 2.2 Modulo 2

En este modulo se utiliza el arreglo creado en el modulo anterior, para luego realizar la suma de cada elemento. Para esto en primer lugar se crea un bloque de código que se encargara de sumar la posiciones de cada thread creado:
```
sumasP = new uint64_t[totalElementos];
for (size_t i = 0; i < numThreads; ++i){
	threads2.push_back(new std::thread(sumaParalela,i,
									i * (totalElementos) / numThreads,
									(i + 1) * (totalElementos) / numThreads));
}
```

Donde luego se sumara el dato guardado por posición para entregar el resultado de la suma esperada:
```
for (size_t i = 0; i < numThreads; i++)

{

resultadoSP += sumasP[i];

}
```
Por ultimo para obtener pruebas de desempeño, se utiliza nuevamente el método` std::chrono`, para obtener el tiempo utilizado para la ejecución de la suma paralela con siguiente sección de código:

```
start = std::chrono::system_clock::now();
for (auto &thr : threads2){
	thr->join();
}
end = std::chrono::system_clock::now();
std::chrono::duration<float, std::milli> duracionSuma = end - start;
auto tiempoSumaP = duracionSuma.count();
```
