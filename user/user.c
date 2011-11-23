#include "rs485.h"
#include "rs485.c"

int main(int argc,char **argv)
{
	
	if(argc == 3)
	{
		if(atoi(argv[1]) == RS485_GPIO_SET_CMD) 
		{
			if((atoi(argv[2]) != RS485_RECEIVE_MODEL) && (atoi(argv[2]) != RS485_TRANS_MODEL))
			{
				fprintf(stdout,"Usage: ./rs485 argv[1] argv[2] here argv[1] must specified to 1 and argv[2] must in 0 or 1\n");
				fprintf(stdout,"argv[1] = 1,set rs485 work model and argv[2] specified model\n");
				return -1;
			}
			else if(argv[2] == 0)
			{
				rs485_set_model(0);
			}
			else
			{
				rs485_set_model(1);
			}
		}
		else
		{
			fprintf(stdout,"Usage: ./rs485 argv[1] argv[2] here argv[1] must specified to 1 and argv[2] must in 0 or 1\n");
			fprintf(stdout,"argv[1] = 1,set rs485 work model and argv[2] specified model\n");
			return -1;
		}
	}
	else if(argc == 2)
	{
		if(atoi(argv[1]) != RS485_GPIO_READ_CMD)
		{
			fprintf(stdout,"Usage: ./rs485 argv[1] argv[1] must to be given 2 to pecified the rs read model\n");
			return -1;
		}
		else
		{
			rs485_get_stats();
		}
	}
	else
	{
		fprintf(stdout,"Usage: ./rs485 [argv[1]] <argv[2]>\n");
		return -1;
	}

	return 0;
}
