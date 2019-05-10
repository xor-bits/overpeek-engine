#include "neuralNetwork.h"

#include <ctime>
#include <iostream>
#include <string>
#include <omp.h>
#include <algorithm>
#include <windows.h>
#include <iomanip>

#include "neuron.h"
#include "math.h"
#include "binaryIO.h"

#include "../Snake/snake_game.h"


neuralNetwork::neuralNetwork(unsigned int init_input_count, unsigned int *layer_sizes, unsigned int layer_count)
{
	u_layer_count = layer_count;
	u_init_input_count = init_input_count;
	p_pcts = std::vector<std::vector<neuron>>();

	for (unsigned int l = 0; l < u_layer_count; l++)
	{
		std::vector<neuron> tmp;
		for (unsigned int s = 0; s < layer_sizes[l]; s++)
		{
			if (l == 0) {
				tmp.push_back(neuron(s + l * 1000000, init_input_count));
			}
			else {
				tmp.push_back(neuron(s + l * 1000000, layer_sizes[l - 1]));
			}
		}
		p_pcts.push_back(tmp);
	}
}

neuralNetwork::~neuralNetwork()
{
}

void neuralNetwork::saveNeuralNetwork(const char* filename) {
	unsigned int weightCount = 0;
	unsigned int biasCount = 0;

	//Get weight and bias counts
	for (unsigned int l = 0; l < p_pcts.size(); l++)
	{
		biasCount += p_pcts[l].size();

		for (unsigned int s = 0; s < p_pcts[l].size(); s++)
		{
			weightCount += p_pcts[l][s].getWeightCount();
		}
	}

	unsigned int *layers = new unsigned int[u_layer_count + 1];
	layers[0] = u_init_input_count;
	float *weights = new float[weightCount];
	float *biases = new float[biasCount];

	//Get weights and biases
	unsigned int neuronWeightIndex = 0;
	unsigned int neuronIndex = 0;
	for (unsigned int l = 0; l < p_pcts.size(); l++)
	{
		layers[l + 1] = p_pcts[l].size();
		for (unsigned int s = 0; s < p_pcts[l].size(); s++)
		{
			++neuronIndex;
			//std::cout << neuronIndex << std::endl;

			biases[neuronIndex] = p_pcts[l][s].getBias();

			for (int i = 0; i < p_pcts[l][s].getWeightCount(); i++)
			{
				++neuronWeightIndex;
				//std::cout << neuronWeightIndex << std::endl;
				weights[neuronWeightIndex] = p_pcts[l][s].getWeight(i);
				
				//I = weight of l layer s neuron
				//TODO: save to weights array
			}
		}
	}

	//std::cout << weightCount << std::endl;
	//std::cout << biasCount << std::endl;
	//std::cout << u_layer_count + 1 << std::endl;

	oe::BinaryIO::write<unsigned int>(std::string(filename) + ".nnl", layers, u_layer_count + 1);
	oe::BinaryIO::write<float>(std::string(filename) + ".nnw", weights, weightCount);
	oe::BinaryIO::write<float>(std::string(filename) + ".nnb", biases, biasCount);
}

neuralNetwork *neuralNetwork::loadNeuralNetwork(const char* filename) {
	unsigned long weightCount = 0;
	unsigned long biasCount = 0;
	unsigned long layerCount = 0;

	std::cout << "Reading..." << std::endl;

	unsigned int *layers = oe::BinaryIO::read<unsigned int>(std::string(filename) + ".nnl", layerCount);
	float *weights = oe::BinaryIO::read<float>(std::string(filename) + ".nnw", weightCount);
	float *biases = oe::BinaryIO::read<float>(std::string(filename) + ".nnb", biasCount);

	if (!layers || !weights || !biases) {
		std::cout << "Couldn't read files." << std::endl;
		return nullptr;
	}

	neuralNetwork *loaded = new neuralNetwork(layers[0], layers + 1, layerCount - 1);

	//Set weights and biases
	unsigned int neuronWeightIndex = 0;
	unsigned int neuronIndex = 0;
	for (unsigned int l = 0; l < loaded->p_pcts.size(); l++)
	{
		layers[l + 1] = loaded->p_pcts[l].size();
		for (unsigned int s = 0; s < loaded->p_pcts[l].size(); s++)
		{
			++neuronIndex;

			loaded->p_pcts[l][s].setBias(biases[neuronIndex]);

			for (int i = 0; i < loaded->p_pcts[l][s].getWeightCount(); i++)
			{
				++neuronWeightIndex;
				loaded->p_pcts[l][s].setWeight(i, weights[neuronWeightIndex]);
			}
		}
	}

	delete[] layers;
	delete[] weights;
	delete[] biases;

	return loaded;
}

void neuralNetwork::applyChanges(float learnRate) {
	for (int i = 0; i < u_layer_count; i++)
	{
		for (int j = 0; j < p_pcts[i].size(); j++)
		{
			p_pcts[i][j].applyChange(learnRate);
		}
	}
}

int neuralNetwork::neuronGuess() {
	int biggest = 0;
	float val_biggest = 0.0f;
	for (int x = 0; x < p_pcts[u_layer_count - 1].size(); x++)
	{
		if (p_pcts[u_layer_count - 1][x].getOutput() > val_biggest) {
			val_biggest = p_pcts[u_layer_count - 1][x].getOutput();
			biggest = x;
		}
	}

	return biggest;
}

void neuralNetwork::operateLayer(float *nauralNetworkInput, unsigned int layer, int finish) {
	for (int s = 0; s < p_pcts[layer].size(); s++)
	{
		if (layer == 0) {
			p_pcts[layer][s].setInputs(nauralNetworkInput);
			p_pcts[layer][s].operate();
		}
		else {
			nauralNetworkInput = new float[p_pcts[layer - 1].size()];
			for (int i = 0; i < p_pcts[layer - 1].size(); i++)
			{
				nauralNetworkInput[i] = p_pcts[layer - 1][i].getOutput();
			}
			p_pcts[layer][s].setInputs(nauralNetworkInput);
			p_pcts[layer][s].operate();
			delete nauralNetworkInput;
		}

		if (finish == sigmoid) p_pcts[layer][s].a_sigmoid();
		if (finish == relu) p_pcts[layer][s].a_relu();
		if (finish == both) { p_pcts[layer][s].a_sigmoid(); p_pcts[layer][s].a_relu(); }
	}
}

void neuralNetwork::dumpInfo() {
	for (unsigned int l = 0; l < u_layer_count; l++)
	{
		for (unsigned int s = 0; s < p_pcts[l].size(); s++)
		{
			p_pcts[l][s].debugInfo();
		}
	}
}

void neuralNetwork::dumpOutputs() {
	for (unsigned int s = 0; s < p_pcts[u_layer_count - 1].size(); s++)
	{
		std::cout << s << ": " << std::setprecision(2) << (float)(p_pcts[u_layer_count - 1][s]).getOutput() << std::endl;
	}
}

void neuralNetwork::mutate(float n) {
	for (int i = 0; i < n; i++)
	{
		unsigned int layer_to_mutate = rand() % u_layer_count;
		unsigned int perceptron_to_mutate = rand() % p_pcts[layer_to_mutate].size();

		p_pcts[layer_to_mutate][perceptron_to_mutate].mutate();
	}
}

float neuralNetwork::combinedError(float *correct_values) {
	float error = 0.0;

	for (unsigned int s = 0; s < p_pcts[u_layer_count - 1].size(); s++)
	{
		error += pow(p_pcts[u_layer_count - 1][s].getOutput() - correct_values[s], 2);
	}
	return error;
}

void neuralNetwork::operateAll(float *nauralNetworkInput) {
	for (int i = 0; i < u_layer_count; i++)
	{
		operateLayer(nauralNetworkInput, i, neuralNetwork::sigmoid);
	}
}

float neuralNetwork::error(unsigned int output, float correct) {
	return pow(p_pcts[u_layer_count - 1][output].getOutput() - correct, 2);
}

unsigned char *f_images;
unsigned char *f_labels;
void readImagesAndLabels() {
	unsigned long n;
	f_images = (unsigned char*)oe::BinaryIO::read<unsigned char>("train-images", n);
	std::cout << "Images: " << n / 784 << std::endl;

	f_labels = (unsigned char*)oe::BinaryIO::read<unsigned char>("train-labels", n);
	std::cout << "Labels: " << n << std::endl;
}

float *getInput(unsigned int index) {
	float *input = new float[784];

	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			input[x + y * 28] = f_images[x + y * 28 + index * 784 + 16] / 255.0f;
		}
	}

	return input;
}

float *getOutput(unsigned int index) {
	float *output = new float[10];
	for (int i = 0; i < 10; i++) output[i] = 0.0f;
	output[f_labels[index + 8]] = 1.0f;

	return output;
}

neuralNetwork *neuralNetwork::geneticAlgorithm(unsigned int* layers, unsigned int layer_count, unsigned int NN_count, unsigned int trainings, unsigned int generations) {
	struct fitnessdata
	{
		float fitness;
		int index;
		std::string move_data;

		bool operator< (const fitnessdata &a) const {
			return fitness < a.fitness;
		}
	};

	srand(time(0));

	const int networks = NN_count;
	const int training_samples = trainings;
	const int max_generations = generations;
	neuralNetwork **neur = new neuralNetwork*[NN_count];
	std::vector<fitnessdata> fitnesses;
	for (int i = 0; i < networks; i++)
	{
		neur[i] = new neuralNetwork(layers[0], layers, layer_count);
		fitnesses.push_back({ 0.5, i, "" });
	}

	fitnessdata best_fitness = { 0.0f, 0 };

	//Train loop
	bool train_complete = false;
	int generation = 0;
	std::string movedata = "";
	while (!train_complete) {

		for (int i = 0; i < networks; i++)
		{
			int timescore = 0;
			snake_game* game = new snake_game(i);
			movedata = "";
			//#pragma omp parallel for
			for (int s = 0; s < training_samples; s++)
			{
				float* input = new float[2];
				glm::vec2 in = game->getSnakeHead();
				//glm::vec2 input2 = game->getFood();
				input[0] = in.x;
				input[1] = in.y;


				neur[i]->operateAll(input);
				delete[] input;
				int key = neur[i]->neuronGuess() + 1;
				movedata += std::to_string(key);
				//std::cout << key << std::endl;
				int feedback = game->input(key);
				if (feedback == 0) timescore += 1;
				if (feedback == 1) timescore += 5;
				//timescore += game->input(key);

				if (game->isGameOver()) {
					break;
				}
			}

			delete game;

			fitnesses[i] = { float(timescore), i, movedata };


			if (fitnesses[i].fitness > best_fitness.fitness) {
				std::sort(fitnesses.begin(), fitnesses.end());
				best_fitness.fitness = fitnesses[i].fitness;
				best_fitness.index = i;
			}
		}

		if (++generation >= max_generations || GetAsyncKeyState(VK_ESCAPE)) {
			train_complete = true;
			continue;
		}

		std::sort(fitnesses.begin(), fitnesses.end());
		std::cout << "Generation: " << generation << std::endl;
		std::cout << "Worst: " << fitnesses[0].fitness << std::endl;
		std::cout << "Worst movedata: " << fitnesses[0].move_data << std::endl << std::endl;
		std::cout << "Best: " << fitnesses[networks - 1].fitness << std::endl;
		std::cout << "Best movedata: " << fitnesses[networks - 1].move_data << std::endl << std::endl << std::endl;


		for (int i = 0; i < networks; i++) {
			if (i != fitnesses[networks - 1].index) {
				delete neur[i];
				neur[i] = new neuralNetwork(*neur[fitnesses[networks - 1].index]);
				neur[i]->mutate(2);
			}
		}
	}
	std::cout << "Trained\n";
	return neur[fitnesses[networks - 1].index];
}

void neuralNetwork::modifyLayer(float *correct_outputs, unsigned int layer) {
	float delta;
	for (int i = 0; i < p_pcts[layer].size(); i++)
	{
		neuron* selected = &p_pcts[layer][i];
		float derivateOfSigmoidFunction = math::sigmoidDerivate(selected->getWeightedSum());
		
		
		if (layer == u_layer_count - 1) {
			float output = p_pcts[layer][i].getOutput();
			float o_error = error(i, correct_outputs[i]);
			delta = 2 * (output - correct_outputs[i]);

			//std::cout << "DELTA: " << delta << ". " << std::endl;
		}
		else {
			delta = selected->requested_change;
		}



		//----
		//Adjust bias of this neuron
		selected->changeBiasWeight(delta * derivateOfSigmoidFunction);


		//----
		//Adjust weights of all connections to this neur
		for (int j = 0; j < selected->getInputCount(); j++)
		{
			float input = selected->getInput(j);
			float change = derivateOfSigmoidFunction * input;
			selected->changeWeight(j, delta * change);
		}

		//----
		//Adjust all neurons connected to this neuron
		//Unless it is first layer (cant change input)
		if (layer >= 1) {
			for (int j = 0; j < selected->getInputCount(); j++)
			{
				float weight = selected->getWeight(j);
				float newDelta = delta * derivateOfSigmoidFunction * weight;
				p_pcts[layer - 1][j].requested_change += newDelta;
			}
		}
	}
}

void neuralNetwork::backpropagation(neuralNetwork &network, unsigned int trainings, float trainingRate) {
	float correct_percentage_var = 0.0f;

	//decoration only
	unsigned int backspace_count = 0; 
	std::cout << "Batch ";

	unsigned int batch_size = 1000;
	for (int i = 1; i < trainings; i++)
	{

//#pragma omp parallel for
		for (int batch_id = 0; batch_id < batch_size; batch_id++)
		{
			float* correct_outputs;
			int index;
			while (true) {
				index = rand() % 60000;
				correct_outputs = getOutput(index);

				if (correct_outputs[0] > 0.5f || correct_outputs[1] > 0.5f) {
					break;
				}
			}
			float* first_layer = getInput(index);

			network.operateAll(first_layer);

			int guess = network.neuronGuess();
			int target = math::argmax(correct_outputs, 10);
			if (guess == target) correct_percentage_var += 1.0f;

			for (int j = network.p_pcts.size() - 1; j >= 0; j--)
			{
				//std::cout << j << std::endl;
				network.modifyLayer(correct_outputs, j);
			}

			//Cleanup
			delete[] first_layer; delete[] correct_outputs;
		}

		//Batch completed
		network.applyChanges(trainingRate);
		std::cout << std::string(backspace_count, '\b');
		std::string text = std::to_string(i) + " : " + std::to_string(correct_percentage_var / (float)batch_size * 100.0f) + "\%";
		backspace_count = text.length();
		std::cout << text;
		correct_percentage_var = 0.0f;
		

		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}
}

neuralNetwork *neuralNetwork::backpropagation(unsigned int trainings, float trainingRate) {
	unsigned int *layers = new unsigned int[3];
	layers[0] = 18;
	layers[1] = 18;
	layers[2] = 10;
	neuralNetwork *network = new neuralNetwork(784, layers, 3);
	backpropagation(*network, trainings, trainingRate);
	return network;
}

int main() {
	srand(time(0));
	readImagesAndLabels();
	unsigned int layers[] = {
		2,
		4
	};
	neuralNetwork *trained_network = neuralNetwork::geneticAlgorithm(layers, 2, 10000, 100, 1000);
	

	//bool trainNoRead = false;
	//std::string train;
	//std::cout << "Train (no reading): ";
	//std::cin >> train;
	//trainNoRead = (int)atof(train.c_str());
	//
	//neuralNetwork *trained_network;
	//if (trainNoRead) {
	//	float inputTrainRate = 0.0;
	//	std::string trainRate;
	//	std::cout << "Training rate: ";
	//	std::cin >> trainRate;
	//
	//	inputTrainRate = atof(trainRate.c_str());
	//
	//	trained_network = neuralNetwork::backpropagation(INFINITE, inputTrainRate);
	//}
	//else {
	//	bool trainAfterRead = false;
	//	std::string trainAfter;
	//	std::cout << "Train after reading: ";
	//	std::cin >> trainAfter;
	//	trainAfterRead = (int)atof(trainAfter.c_str());
	//
	//	trained_network = neuralNetwork::loadNeuralNetwork("backpropped");
	//
	//	if (trainAfterRead) {
	//		float inputTrainRate = 0.0;
	//		std::string trainRate;
	//		std::cout << "Training rate: ";
	//		std::cin >> trainRate;
	//
	//		inputTrainRate = atof(trainRate.c_str());
	//
	//		neuralNetwork::backpropagation(*trained_network, INFINITE, inputTrainRate);
	//	}
	//}
	//trained_network->saveNeuralNetwork("backpropped");
	//
	//while (true) {
	//	float inputX = 0.0;
	//	float inputY = 0.0;
	//	bool infoDump = false;
	//
	//	std::string in1;
	//	std::cout << "Input X: ";
	//	std::cin >> in1;
	//
	//	//std::string in2;
	//	//std::cout << "Input Y: ";
	//	//std::cin >> in2;
	//	//
	//	std::string in3;
	//	std::cout << "Info dump: ";
	//	std::cin >> in3;
	//
	//	inputX = atof(in1.c_str());
	//	//inputY = atof(in2.c_str());
	//	infoDump = (int)atof(in3.c_str());
	//
	//	float *first_layer = getInput(inputX);
	//	float *correct_outputs = getOutput(inputX);
	//
	//	trained_network->operateAll(first_layer);
	//
	//	//std::cout << "INPUT: " << std::endl;
	//	//for (int y = 0; y < 28; y++)
	//	//{
	//	//	for (int x = 0; x < 28; x++)
	//	//	{
	//	//		std::cout << std::setprecision(3) << first_layer[x + y * 28] * 255.0 << "\t";
	//	//	}
	//	//	std::cout << std::endl;
	//	//}
	//	//
	//	int biggest = 0;
	//	float val_biggest = 0.0f;
	//	for (int x = 0; x < 10; x++)
	//	{
	//		if (correct_outputs[x] > val_biggest) {
	//			val_biggest = correct_outputs[x];
	//			biggest = x;
	//		}
	//	}
	//	std::cout << std::endl;
	//	std::cout << "EXPECTED OUTPUT: " << biggest << std::endl;
	//
	//	std::cout << "OUTPUT: " << trained_network->neuronGuess() << std::endl;
	//	
	//
	//	//trained_network->dumpOutputs();
	//
	//
	//	if (infoDump) trained_network->dumpInfo();
	//
	//	delete first_layer;
	//	delete correct_outputs;
	//	//system("pause");
	//}
}




//WDLIST: for while if else int char unsigned long signed inline void float double