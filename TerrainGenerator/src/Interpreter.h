#include <string>
#include <string.h>

using namespace std;

namespace Interpeter{
	string expression = "";

	enum op
	{
		ADD, SUB, MUL, DIV, NONE
	};

	void setExpression(string e){
		expression = e;
	}

	float* getValue(float x, float y, float z){
		if (expression == ""){
			return NULL;
		}

		float result = 0.0f;

		char exp[] = expression.c_str;
		char *token = strtok(exp, " ");
		op cur_op = NONE;

		while (token != NULL){
			float current = 1.0f;
			if (token[0] == '-')
				current = -1.0f;

			int num_char = strlen(exp);
			for (int i = 0; i < num_char; i++){
				switch (token[i]){
				case 'x':
					current *= x;
					break;
				case 'y':
					current *= y;
					break;
				case 'z':
					current *= z;
					break;
				}
			}

		}

		return &result;
	}
}