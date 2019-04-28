#include "neuron.h"

#include <cmath>
//#include <algorithm>
#include <iostream>
#include <string>

#include "math.h"

neuron::neuron(int id, unsigned int input_count)
{
	i_id = id;
	u_input_count = input_count;
	f_bias_weight = math::random(-1.0f, 1.0f);
	f_bias_weight_change = 0.0f;
	f_hold = 0.0f;
	f_weighted_sum = 0.0f;
	requested_change = 0.0f;

	for (int i = 0; i < input_count; i++)
	{
		f_inputs.push_back(0.0f);
		f_weights.push_back(math::random(-1.0f, 1.0f) / input_count);
		f_weight_changes.push_back(0.0);
	}
}

neuron::~neuron()
{
}

void neuron::applyChange(float learnRate) {
	for (unsigned int i = 0; i < u_input_count; i++)
	{
		f_weights[i] -= learnRate * f_weight_changes[i];
		f_weight_changes[i] = 0.0f;
	}
	f_bias_weight -= learnRate * f_bias_weight_change;
	f_bias_weight_change = 0.0f;
}

void neuron::operate() {
	f_weighted_sum = 0.0f;
	for (unsigned int i = 0; i < u_input_count; i++)
	{
		f_weighted_sum += f_inputs[i] * f_weights[i];
	}
	f_weighted_sum += 1.0f * f_bias_weight;
	f_hold = f_weighted_sum;
}

void neuron::setInputs(float *inputs) {
	for (unsigned int i = 0; i < u_input_count; i++)
	{
		f_inputs[i] = inputs[i];
	}
}

void neuron::a_sigmoid() {
	f_hold = math::sigmoid(f_hold);
	//std::cout << f_hold << std::endl;
}

void neuron::a_relu() {
	f_hold = math::relu(f_hold);
}

void neuron::debugInfo() {
	std::string *weight_text = new std::string();
	for (int i = 0; i < u_input_count; i++)
	{
		std::string tmp = std::string("Weight ") + std::to_string(i) + ": " + std::to_string(f_weights[i]) + "\n";
		weight_text->append(tmp);
	}

	std::cout << "Hold " << f_hold << std::endl;
	std::cout << "\n\n\nPerceptron ID " << i_id << std::endl;
	printf(weight_text->c_str());
	std::cout << "Bias weight " << f_bias_weight << std::endl;
}

void neuron::mutate() {
	int mutation_size = pow(0.1, (rand() % 3));

	float mutation = math::random(-mutation_size, mutation_size);

	bool bias_to_mutate = floor(math::random(0.0f, 1.99999f));


	if (bias_to_mutate) {
		f_bias_weight += mutation;
	}
	else {
		unsigned int weight_to_mutate = rand() % u_input_count;
		f_weights[weight_to_mutate] += mutation;
	}
}