#include <global.h>
#include <checkArgs.hpp>
uint64_t sumasS = 0;
uint64_t *sumasP = nullptr;
uint64_t *arreglo_c = nullptr;
uint64_t *arregloEnParalelo = nullptr;
uint64_t *arregloEnSerie = nullptr;

void sumaSerie(size_t pos,
				size_t beginIndex,
				size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sumasS += arregloEnParalelo[i];
	}
}
void sumaParalela(size_t pos,
				  size_t beginIndex,
				  size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sumasP[pos] += arregloEnParalelo[i];
	}
}

void LlenadoDelArreglo(size_t beginIdx, size_t endIdx, uint32_t limiteInferior, uint32_t limiteSuperior, size_t Rnetype)
{
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<uint32_t> nRandom(limiteInferior, limiteSuperior);
	for (size_t i = beginIdx; i < endIdx; ++i)
	{
		uint32_t random = nRandom(rng);
		switch (Rnetype)
		{
		case 0:
			arregloEnParalelo[i] = random;

			break;

		case 1:
			arregloEnSerie[i] = random;
			break;
		}
	}
}

int main(int argc, char **argv)
{

	uint64_t totalElementos;
	uint64_t numThreads;
	uint32_t limInf;
	uint32_t limSup;
	uint64_t resultadoSP = 0;
	std::vector<std::thread *> threads;
	std::vector<std::thread *> threads2;

	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	limInf = argumentos->getArgs().limInferior;
	limSup = argumentos->getArgs().limSuperior;

//Llenado del arreglo de forma secuencial

	arregloEnSerie = new uint64_t[totalElementos];
	auto start = std::chrono::high_resolution_clock::now();
	LlenadoDelArreglo(0, totalElementos, limInf, limSup, 1);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto tiempoArrayS = elapsed.count();

//Llenado del arreglo de forma paralela

	arregloEnParalelo = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.push_back(new std::thread(LlenadoDelArreglo,
										  i * (totalElementos) / numThreads,
										  (i + 1) * (totalElementos) / numThreads, limInf, limSup, 0));
	}
//Tiempo de llenado de forma paralela

	start = std::chrono::system_clock::now();
	for (auto &thrs : threads)
	{
		thrs->join();
	}
	end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duracionLlenado = end - start;
	auto tiempoArrayP = duracionLlenado.count();

//Suma de Threads

	sumasP = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{

		threads2.push_back(new std::thread(sumaParalela,
										   i,
										   i * (totalElementos) / numThreads,
										   (i + 1) * (totalElementos) / numThreads));
	}

//Tiempo de suma de los Threads

	start = std::chrono::system_clock::now();
	for (auto &thr : threads2)
	{
		thr->join();
	}
	end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duracionSuma = end - start;
	auto tiempoSumaP = duracionSuma.count();

//Tiempo de sumaSecuencial

	start = std::chrono::high_resolution_clock::now();
	LlenadoDelArreglo(0, totalElementos, limInf, limSup, 1);
	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto tiempoSumaS = elapsed.count();

//Suma de forma secuencial

	sumaSerie(0,0,totalElementos);
	
//Suma de forma Paralela

	for (size_t i = 0; i < numThreads; i++)
	{
		resultadoSP += sumasP[i];
	}
	
	std::cout << "=======LLenado y Suma de numeros de un Arreglo======= " << std::endl;
	
	std::cout << "Tiempo de LLenado secuencial: " << tiempoArrayS << "[ms]" << std::endl;
	std::cout << "Tiempo de LLenado con threads: " << tiempoArrayP << "[ms]" << std::endl;
	std::cout << "SpeedUp de suma: " << (double) tiempoArrayS/tiempoArrayP << std::endl;

	std::cout << "================================================= " << std::endl;
	
	std::cout << "suma secuencial: " << sumasS << std::endl;
	std::cout << "suma con threads: " << resultadoSP << std::endl;
	
	std::cout << "================================================= " << std::endl;

	std::cout << "Tiempo de suma secuencial: " << tiempoSumaS << "[ms]"<< std::endl;
	std::cout << "Tiempo de suma con Threads: " << tiempoSumaP << "[ms]"<< std::endl;
	std::cout << "SpeedUp de suma: " << (double) tiempoSumaS/tiempoSumaP << std::endl;
	

	return(EXIT_SUCCESS);
}

