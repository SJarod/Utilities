#include "utils/ai/perceptron.hpp"

Utils::AI::Perceptron::Perceptron(int numInput, ActivationPtr func, Perceptron* nextPerceptron)
	: func(func), nextP(nextPerceptron)
{
	inputs.resize(numInput);
}

float Utils::AI::Perceptron::sum() const
{
	float sum = 0.f;
	for (Input i : inputs)
	{
		sum += i.value * i.weight;
	}

	return sum;
}

void Utils::AI::Perceptron::feed(int index, float input)
{
	inputs[index].value = input;
}

float Utils::AI::Perceptron::process(int selfIndex) const
{
	float output = (*func)(sum());
	if (nextP)
		nextP->feed(selfIndex, output);

	return output;
}