#pragma once

#include <vector>

#include "activation.hpp"

namespace Utils::AI
{
	// neuron
	class Perceptron
	{
	private:
		struct Input
		{
			float value = 0.f;
			// TODO : include Mathematics
			// float weight = Math::randomf(-0.1f, 0.1f);
			float weight = 0.1f;
		};

		std::vector<Input> inputs;

		// activation function
		ActivationPtr func;

		std::vector<Perceptron*> nextPerceptrons;

		float sum() const;

	public:
		// result (state) of this perceptron after processing
		float output = 0.f;

		// error rate
		float delta = 0.f;

		Perceptron(int numInput, ActivationPtr func);

		void addNextPerceptron(Perceptron* nextP);

		/**
		 * Set this perceptron's input value.
		 */
		void feed(int index, float input);

		/**
		 * Summum the inputs than go through activation function (do not forget to feed).
		 */
		float process(int selfIndex);

		/**
		 * Adjust this perceptron's weights.
		 * 
		 * @param float alpha is the learning rate
		 */
		void adjustWeights(const float alpha);

		const std::vector<Perceptron*>& getNextPerceptrons() const;

		float getInputWeight(const int inputIndex) const;
	};
}