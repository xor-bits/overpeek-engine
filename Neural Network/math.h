#pragma once

namespace math {

	float gaussRandom(float min, float max);
	float random(float min, float max);
	float sigmoid(float val);
	float relu(float val);
	float sigmoidDerivate(float val);
	float reluDerivate(float val);

	template <class T> int argmax(T* t_array, size_t count) {
		int index_with_with_greatest_value = 0;
		float greatest_value = 0.0f;
		for (size_t i = 0; i < count; i++)
		{
			if (t_array[i] > greatest_value) {
				greatest_value = t_array[i];
				index_with_with_greatest_value = i;
			}
		}

		return index_with_with_greatest_value;
	}

}