#pragma once

#include <vector>

class neuron
{
private:
	int i_id;

	float f_hold;

	unsigned int u_input_count;
	std::vector<float> f_weights;
	float f_bias_weight;

	std::vector<float> f_weight_changes;
	float f_bias_weight_change;

	float f_weighted_sum;
	std::vector<float> f_inputs;

public:
	float requested_change;


public:
	neuron(int id, unsigned int input_count);
	~neuron();

	void setInputs(float *inputs);

	void operate();
	void mutate();

	void debugInfo();

	void a_sigmoid();
	void a_relu();
	void applyChange(float learnRate);

	inline void changeWeight(unsigned int i, float change) { f_weight_changes[i] += change; }
	inline void changeBiasWeight(float change) { f_bias_weight_change += change; }

	inline float getWeightedSum() { return f_weighted_sum; }
	inline float getWeight(unsigned int i) { return f_weights[i]; }
	inline void setWeight(unsigned int i, float value) { f_weights[i] = value; }
	inline unsigned int getWeightCount() { return f_weights.size(); }

	inline float getBias() { return f_bias_weight; }
	inline void setBias(float value) { f_bias_weight = value; }

	inline unsigned int getInputCount() const { return u_input_count; }
	inline float getInput(unsigned int i) { return f_inputs[i]; }

	inline float getOutput() { return f_hold; }
};

