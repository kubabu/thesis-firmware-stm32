/*
 * nn_classifier.c
 *
 *  Created on: Sep 4, 2017
 *      Author: kuba
 */

#include <stdio.h>

#include "classifiers.h"

#define OUT_CHANNELS 3
#define KERNEL_SIZE 5
#define POOL_SIZE 5
#define AFTER_POOL_LEN (SEQUENCE_LEN / POOL_SIZE)


const int seq_len = SEQUENCE_LEN;


const float kernel[OUT_CHANNELS][FEATURES][KERNEL_SIZE] = {
    {
        {0.12823f, 0.03114f, -0.11964f, -0.10511f, -0.00914f },
        {-0.01131f, 0.01349f, -0.00665f, -0.07268f, -0.04067f },
        {0.12378f, -0.01874f, -0.06571f, -0.07875f, 0.10947f },
        {-0.08509f, -0.04827f, -0.10535f, -0.00040f, -0.04302f },
        {-0.03716f, 0.03913f, -0.10991f, -0.07073f, 0.07761f },
        {-0.06570f, -0.03221f, -0.11654f, 0.06016f, 0.01798f },
        {0.05417f, 0.07170f, -0.00735f, -0.08622f, 0.12284f },
        {0.10647f, 0.07003f, -0.03909f, 0.03181f, 0.10529f },
        {-0.12897f, -0.02286f, -0.10251f, -0.00590f, -0.02741f },
        {0.02718f, -0.02591f, 0.04223f, 0.08878f, -0.05173f },
        {0.05094f, -0.03589f, 0.03531f, 0.05254f, 0.12909f },
        {0.12083f, -0.11457f, -0.04341f, -0.06373f, 0.08321f }
    },
    {
        {-0.10080f, 0.06351f, -0.03699f, -0.11418f, 0.12076f },
        {0.01778f, -0.07566f, -0.04002f, -0.07612f, -0.09414f },
        {-0.02577f, -0.05241f, -0.10655f, -0.00799f, 0.05895f },
        {-0.07722f, -0.11889f, 0.01204f, -0.04888f, 0.01256f },
        {0.02981f, 0.07095f, 0.02824f, -0.05311f, -0.04926f },
        {0.10600f, -0.09991f, -0.04132f, -0.10557f, -0.01075f },
        {-0.00451f, -0.05755f, 0.00484f, -0.12425f, 0.00071f },
        {-0.00476f, 0.01315f, 0.04486f, -0.00216f, -0.10633f },
        {-0.03604f, 0.05952f, -0.07882f, -0.03627f, -0.01443f },
        {0.05572f, -0.04397f, -0.11785f, -0.07224f, -0.08193f },
        {-0.10245f, -0.06195f, -0.01667f, 0.10504f, 0.10311f },
        {0.09449f, 0.12334f, -0.05706f, -0.09627f, 0.00691f }
    },
    {
        {0.02619f, -0.04328f, -0.11392f, 0.12063f, -0.00743f },
        {-0.01276f, 0.06656f, -0.07417f, -0.02010f, 0.11852f },
        {0.06948f, 0.07133f, 0.01942f, -0.09746f, -0.01660f },
        {0.00356f, 0.08198f, -0.03438f, -0.11107f, -0.07795f },
        {0.02503f, -0.04795f, 0.05380f, -0.05161f, -0.11958f },
        {-0.03415f, -0.04117f, 0.00258f, -0.06408f, -0.10689f },
        {0.12724f, 0.10256f, 0.00778f, -0.02123f, 0.01370f },
        {0.12832f, -0.12576f, -0.07046f, 0.01697f, 0.03327f },
        {0.06656f, 0.04765f, 0.02764f, -0.12821f, 0.11416f },
        {0.01877f, -0.00859f, 0.08205f, 0.06508f, -0.02302f },
        {0.06821f, 0.01181f, 0.05435f, -0.09987f, 0.04946f },
        {0.06425f, -0.02391f, -0.10275f, -0.01426f, -0.01397f }
    }
};

float bias[OUT_CHANNELS] = {
    0.00700f, 0.10914f, 0.06478f
};

float output[OUT_CHANNELS][SEQUENCE_LEN] = {{0.0f}};

float pool_output[OUT_CHANNELS][AFTER_POOL_LEN] = {{0.0f}};

float relu_output[OUT_CHANNELS][AFTER_POOL_LEN] = {{0.0f}};


void print_seq(const char *name, float *seq, int len)
{
    puts(name);
    for(int i = 0; i < len; i++)
    {
        printf("%.4f ", seq[i]);
    }

    putchar('\n');
}


float seq_max(float *seq, int size)
{
    float max = seq[0];
    for(int i = 1; i < size; i++)
    {
        if(seq[i] > max)
        {
            max = seq[i];
        }
    }
    return max;
}

int16_t run_nn_classifier(rbuf_iterator_t data_series[DTW_FEATURES])
{
    for(int c_idx = 0; c_idx < OUT_CHANNELS; c_idx++)
	{
		for(int s_idx = 0; s_idx < SEQUENCE_LEN; s_idx++)
		{
			for(int f_idx = 0; f_idx < FEATURES; f_idx++)
			{
				for(int k_idx = 0; k_idx < KERNEL_SIZE; k_idx++)
				{
					float left = iterate(&data_series[f_idx], s_idx + k_idx);
					float right = kernel[c_idx][f_idx][k_idx];

					output[c_idx][s_idx] += left * right;

					//printf("%.1f + %.1f * %.1f = %.1f\n", orig, left,right, output[c_idx][s_idx]);
				}
			}
			// add bias term
			output[c_idx][s_idx] += bias[c_idx];
		}

		for(int i = 0; i < AFTER_POOL_LEN; i++)
		{
			// pointer arithmetic
			float val = seq_max(&(output[c_idx][i * POOL_SIZE]), POOL_SIZE);
			pool_output[c_idx][i] = val;
		}

		//relu
		for(int i = 0; i < AFTER_POOL_LEN; i++)
		{
			if(pool_output[c_idx][i] > 0)
			{
				relu_output[c_idx][i] = pool_output[c_idx][i];
			}
		}

		print_seq("seq", relu_output[c_idx], AFTER_POOL_LEN);
		putchar('\n');
    }

    return 0;
}
