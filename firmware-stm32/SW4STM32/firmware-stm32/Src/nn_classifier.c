/*
 * nn_classifier.c
 *
 *  Created on: Sep 4, 2017
 *      Author: kuba
 */

#include <stdio.h>

#include "classifiers.h"

#define OUT_CHANNELS 3
#define FEATURES 12
#define SEQ_LEN 50
#define PADDING 2
#define PADDED_SEQ_LEN 54
#define KERNEL_SIZE 5
#define POOL_SIZE 5
#define AFTER_POOL_LEN (SEQ_LEN / POOL_SIZE)


const int seq_len = SEQ_LEN;


float series[FEATURES][PADDED_SEQ_LEN] = {
    { 0.0f, 0.0f,-0.58925f, -0.61415f, -0.58303f, -0.62452f, -0.63697f, -0.61000f, -0.57265f, -0.62037f, -0.59547f, -0.62452f, -0.61207f, -0.61207f, -0.63697f, -0.63490f, -0.70129f, -0.66187f, -0.63905f, -0.79673f, -0.95649f, -0.68677f, -0.56228f, 0.37969f, 0.70336f, 0.35686f, -0.07262f, -0.68884f, -1.28016f, -1.65778f, -2.09349f, -2.40471f, -2.35284f, -2.42753f, -2.09349f, -1.87978f, -1.67230f, -1.30298f, -0.73656f, -0.09752f, 0.37969f, 0.19295f, -0.03113f, -0.15354f, -0.40667f, -0.35895f, -0.46891f, -0.52908f, -0.63075f, -0.64527f, -0.61000f, -0.60170f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.19833f, 0.16430f, 0.18596f, 0.17358f, 0.17358f, 0.20761f, 0.21998f, 0.18596f, 0.20761f, 0.19524f, 0.18286f, 0.26638f, 0.25091f, 0.31896f, 0.34371f, 0.29112f, 0.23854f, 0.08388f, -0.07696f, -0.67703f, -1.33897f, -1.57095f, -1.71633f, -1.44104f, -1.46888f, -1.45341f, -1.39464f, -1.36680f, -1.42248f, -1.30803f, -1.14719f, -1.12245f, -1.15338f, -1.03893f, -0.97707f, -0.88118f, -0.69868f, -0.36153f, -0.13573f, -0.03675f, 0.02511f, 0.01583f, 0.09316f, 0.10863f, 0.11172f, 0.07460f, 0.13028f, 0.09626f, 0.12100f, 0.12100f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.52463f, 0.54311f, 0.52463f, 0.56774f, 0.50821f, 0.53900f, 0.55132f, 0.51642f, 0.54105f, 0.53695f, 0.49385f, 0.55337f, 0.56568f, 0.59031f, 0.47127f, 0.33580f, 0.17160f, -0.14449f, -0.61042f, -1.08455f, -0.97988f, 1.16913f, 1.70895f, 2.81937f, 2.31239f, 2.17077f, 1.84236f, 1.58169f, 1.46880f, 1.04187f, 0.66421f, 0.69089f, 0.37890f, 0.30501f, 0.04024f, -0.13218f, 0.02997f, -0.10344f, 0.16133f, 0.36864f, 0.59237f, 0.77094f, 0.75862f, 0.71757f, 0.65600f, 0.50616f, 0.63342f, 0.58005f, 0.52463f, 0.57800f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.15635f, 0.14590f, 0.15635f, 0.15407f, 0.17250f, 0.14704f, 0.17953f, 0.18067f, 0.19454f, 0.21430f, 0.19226f, 0.18409f, 0.20157f, 0.23633f, 0.27110f, 0.23386f, 0.13545f, -0.07409f, -0.45405f, -1.20237f, -2.51494f, -2.65970f, -2.57060f, -1.73754f, -0.47856f, 0.23158f, 0.71812f, 0.84787f, 1.22441f, 1.83823f, 1.77345f, 1.83709f, 1.69005f, 1.57416f, 1.47689f, 0.98580f, 0.68810f, 0.32315f, -0.01273f, -0.04977f, -0.15521f, -0.04977f, 0.07865f, 0.07523f, 0.20727f, 0.18637f, 0.12862f, 0.21544f, 0.15407f, 0.17250f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.14138f, 0.13767f, 0.16710f, 0.15349f, 0.14138f, 0.13544f, 0.14521f, 0.14978f, 0.13235f, 0.14892f, 0.13161f, 0.12184f, 0.12715f, 0.13915f, 0.17464f, 0.23116f, 0.28545f, 0.40688f, 0.66473f, 1.09310f, 0.82079f, 0.11132f, -0.01840f, -0.09087f, 0.62849f, 0.35779f, 0.53574f, 0.69119f, 0.51620f, 0.54564f, 0.02686f, 0.17835f, 0.05704f, -0.01395f, 0.08560f, 0.20927f, 0.07126f, -0.01234f, 0.15275f, 0.45363f, 0.30202f, 0.19047f, 0.14521f, 0.10378f, 0.13012f, 0.11578f, 0.12258f, 0.14892f, 0.13989f, 0.16932f,  0.0f, 0.0f },
    { 0.0f, 0.0f,-0.00760f, -0.00985f, -0.00195f, -0.00647f, 0.00162f, 0.01986f, -0.00760f, 0.00613f, 0.01873f, 0.01986f, 0.03472f, 0.04732f, 0.08173f, 0.11164f, 0.17125f, 0.19307f, 0.20228f, 0.21714f, 0.33750f, 0.33411f, 0.58066f, 0.24234f, -1.18881f, -2.07401f, -2.81929f, -2.67486f, -2.50279f, -2.12779f, -1.44100f, -0.98683f, -0.18080f, 0.50957f, 1.11964f, 1.52773f, 1.82713f, 2.20551f, 2.41877f, 1.98416f, 1.42580f, 0.85015f, 0.53252f, 0.24121f, 0.10938f, 0.02908f, -0.03392f, -0.07417f, -0.04897f, -0.02715f, 0.00162f, 0.01065f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.59806f, 0.59563f, 0.59903f, 0.59466f, 0.59223f, 0.59611f, 0.60000f, 0.59466f, 0.59757f, 0.59417f, 0.59514f, 0.59611f, 0.59320f, 0.59417f, 0.58154f, 0.58397f, 0.58154f, 0.50044f, 0.13282f, -0.66362f, 0.18721f, 0.54852f, 0.52667f, 0.58980f, 0.60631f, 0.60388f, 0.54998f, 0.47519f, 0.37029f, 0.20129f, 0.09931f, 0.08328f, 0.08037f, 0.13670f, 0.10562f, 0.21975f, 0.54755f, 0.57523f, 0.43780f, 0.53347f, 0.58931f, 0.60388f, 0.61020f, 0.61068f, 0.60777f, 0.60340f, 0.59514f, 0.59271f, 0.59563f, 0.59757f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.05633f, 0.01973f, 0.04992f, 0.00692f, 0.04718f, 0.05129f, 0.05358f, 0.04992f, 0.05129f, 0.04352f, 0.06547f, 0.08011f, 0.05998f, 0.08697f, 0.17755f, 0.25212f, 0.36053f, 0.51058f, 0.78368f, 1.24799f, -0.07771f, -1.49581f, -1.58319f, -1.45418f, -1.45510f, -1.44458f, -1.40661f, -1.38648f, -1.42216f, -1.35080f, -1.22775f, -1.22180f, -1.18474f, -1.05986f, -0.91851f, -0.65822f, -0.36682f, 0.14278f, -0.04157f, -0.05255f, -0.11476f, -0.21677f, -0.15319f, -0.11934f, -0.08045f, -0.00909f, -0.06490f, -0.05484f, 0.00143f, -0.03288f,  0.0f, 0.0f },
    { 0.0f, 0.0f,-0.59674f, -0.61873f, -0.59149f, -0.62496f, -0.64137f, -0.61381f, -0.57935f, -0.62529f, -0.60035f, -0.62628f, -0.62037f, -0.61184f, -0.63350f, -0.62562f, -0.69289f, -0.67550f, -0.67615f, -0.90880f, -1.26810f, -1.28714f, -1.54669f, 0.16748f, 0.25771f, -0.06517f, -0.24171f, -0.57410f, -0.93964f, -1.20477f, -1.46466f, -1.76719f, -1.90665f, -1.92765f, -1.92831f, -1.83774f, -1.85907f, -1.65366f, -0.89666f, -0.05073f, 0.37486f, 0.13237f, -0.10750f, -0.22530f, -0.43399f, -0.39560f, -0.48879f, -0.54982f, -0.62661f, -0.64564f, -0.61939f, -0.60626f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.67185f, 0.67176f, 0.67170f, 0.67165f, 0.67159f, 0.67165f, 0.67161f, 0.67156f, 0.67161f, 0.67167f, 0.67178f, 0.67194f, 0.67223f, 0.67272f, 0.67336f, 0.67422f, 0.67515f, 0.67606f, 0.67739f, 0.67908f, 0.68085f, 0.68333f, 0.68001f, 0.67154f, 0.65964f, 0.64630f, 0.63404f, 0.62316f, 0.61509f, 0.60940f, 0.60720f, 0.60833f, 0.61259f, 0.61902f, 0.62710f, 0.63686f, 0.64805f, 0.65795f, 0.66571f, 0.67052f, 0.67356f, 0.67520f, 0.67586f, 0.67622f, 0.67613f, 0.67582f, 0.67553f, 0.67533f, 0.67526f, 0.67529f,  0.0f, 0.0f },
    { 0.0f, 0.0f,0.10554f, 0.10653f, 0.10851f, 0.10455f, 0.10455f, 0.10504f, 0.10554f, 0.10703f, 0.10851f, 0.11149f, 0.11595f, 0.11843f, 0.12339f, 0.13033f, 0.15066f, 0.17794f, 0.20868f, 0.23695f, 0.26026f, 0.28753f, 0.13232f, -0.33680f, -0.74045f, -1.05733f, -1.20957f, -1.26312f, -1.26362f, -1.23436f, -1.18279f, -1.08807f, -0.97551f, -0.88079f, -0.78955f, -0.70971f, -0.63681f, -0.58375f, -0.54061f, -0.47862f, -0.44093f, -0.42407f, -0.41862f, -0.42259f, -0.41366f, -0.39184f, -0.36010f, -0.31696f, -0.28969f, -0.25845f, -0.22373f, -0.19844f,  0.0f, 0.0f },
    { 0.0f, 0.0f,-0.55478f, -0.55606f, -0.55606f, -0.55733f, -0.56117f, -0.56405f, -0.56245f, -0.56405f, -0.56597f, -0.56853f, -0.57077f, -0.57269f, -0.57525f, -0.58068f, -0.58644f, -0.58516f, -0.58036f, -0.58132f, -0.60659f, -0.61171f, -0.65809f, -0.54774f, -0.43387f, -0.41020f, -0.35583f, -0.33919f, -0.36094f, -0.39773f, -0.46937f, -0.57013f, -0.73262f, -0.89286f, -1.03552f, -1.16602f, -1.27669f, -1.36529f, -1.38896f, -1.30100f, -1.14011f, -0.96867f, -0.84360f, -0.76172f, -0.71630f, -0.68176f, -0.65617f, -0.63922f, -0.63858f, -0.63506f, -0.63186f, -0.62674f, 0.0f, 0.0f }
};

float kernel[OUT_CHANNELS][FEATURES][KERNEL_SIZE] = {
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

float output[OUT_CHANNELS][SEQ_LEN] = {{0.0f}};

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

uint16_t run_nn_classifier(float** data_series)
{
    for(int c_idx = 0; c_idx < OUT_CHANNELS; c_idx++)
	{
		for(int s_idx = 0; s_idx < SEQ_LEN; s_idx++)
		{
			for(int f_idx = 0; f_idx < FEATURES; f_idx++)
			{
				for(int k_idx = 0; k_idx < KERNEL_SIZE; k_idx++)
				{
					float left = data_series[f_idx][s_idx + k_idx];
					float right = kernel[c_idx][f_idx][k_idx];
					//float orig = output[c_idx][s_idx];

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