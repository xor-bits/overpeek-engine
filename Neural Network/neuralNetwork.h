#pragma once

#include <vector>

class neuron;
class neuralNetwork
{
private:
	unsigned int u_layer_count;
	std::vector<std::vector<neuron>> p_pcts;
	unsigned int u_init_input_count;

	float f_images;
	float f_labels;

public:
	enum finish {
		sigmoid, relu, neither, both
	};

	neuralNetwork(unsigned int init_input_count, unsigned int *layer_sizes, unsigned int layer_count);
	neuralNetwork() {}
	~neuralNetwork();

	void saveNeuralNetwork(const char* filename);
	static neuralNetwork *loadNeuralNetwork(const char* filename);

	void operateLayer(float *nauralNetworkInput, unsigned int layer, int finish);
	void operateAll(float *nauralNetworkInput);
	void dumpInfo();
	void dumpOutputs();
	int neuronGuess();
	
	void mutate(float n);
	void applyChanges(float learnRate);

	float combinedError(float *correct_values);
	float error(unsigned int output, float correct);

	void modifyLayer(float *correct_outputs, unsigned int layer);

public:
	static neuralNetwork *geneticAlgorithm(unsigned int NN_count, unsigned int trainings, unsigned int generations);
	static neuralNetwork *backpropagation(unsigned int trainings, float trainingRate);
	static void backpropagation(neuralNetwork &network, unsigned int trainings, float trainingRate);

};