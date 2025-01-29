






#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <termios.h>


#define MAX_BUFFER_SIZE 10


struct operator_struct {
	int operator_materials[2];
	int operator_tools[2];
};


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int buffer_counter = 0;
int material1_counter = 0;
int material2_counter = 0;
int material3_counter = 0;
int material1_in_buffer = 0;
int material2_in_buffer = 0;
int material3_in_buffer = 0;


int tools_counter = 0;


int output_queue_counter = 0;


int productA_counter = 0;
int productB_counter = 0;
int productC_counter = 0;


int initial_grab = 1;




struct Node
{
    int data;
    struct Node* next;
};


int countElements(struct Node **head)
{
    struct Node * current = *head;
    int length = 0;
    
    while (current != NULL)
    {
        length++;
        current = current->next;
    }
    return length;
}


void printList(struct Node **head)
{
    struct Node * current = *head;

    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}


void printOutputQueue(struct Node **head)
{
	struct Node * current = *head;

	while (current != NULL)
	{
		if (current->data == 111)
			printf("A ");
		else if (current->data == 222)
			printf("B ");
		else if (current->data == 333)
			printf("C ");

		current = current->next;
	}
	printf("\n");
}


void appendNode(struct Node **head, int data)
{
    struct Node * current = *head;
    struct Node * newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    if (current == NULL)
    {
        *head = newNode;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}


int removeNode(struct Node **head, int index)
{
    struct Node * temp = *head;
    int ret;

    
    if (index == 0)
    {
        ret = temp->data;
        *head = temp->next;
        free(temp);
        return ret;
    }

    
    for (int i = 0; temp != NULL && i < index-1; i++)
    {
        temp = temp->next;
    }

    
    
    struct Node * next = temp->next->next;
    ret = temp->next->data;
    
    free(temp->next);
    
    temp->next = next;
    return ret;
}


int getNodeData(struct Node **head, int index)
{
	struct Node * current = *head;
	int count = 0;
	int ret = -1;

	
	if (index == 0)
	{
		ret = current->data;
		return ret;
	}

	while (current != NULL)
	{
		if (count == index)
		{
			ret = current->data;
			return ret;
		}
		count++;
		current = current->next;
	}

	
	return ret;
}



struct Node * materials = NULL;

struct Node * tools = NULL;

struct Node * output_queue = NULL;


int insert_material(int material)
{
	
	if (buffer_counter < MAX_BUFFER_SIZE)
	{
		
		if (material == 1)
			material1_in_buffer++;
		else if (material == 2)
			material2_in_buffer++;
		else if (material == 3)
			material3_in_buffer++;

		appendNode(&materials, material);
		buffer_counter++;
		printf("- Parameter 2 has changed -\n");
		printf("Material %d has been added into input buffer\n", material);
		printf("Input Buffer: ");
		printList(&materials);
		printf("Input Buffer Size: %d\n", buffer_counter);
		printf("\n");
		return 0;
	}
	
	else
	{
		return -1;
	}
}


int remove_material(int position)
{
	int material = getNodeData(&materials, position);
	
	if (buffer_counter > 0)
	{
		
		if (material == 1)
			material1_in_buffer--;
		else if (material == 2)
			material2_in_buffer--;
		else if (material == 3)
			material3_in_buffer--;

		removeNode(&materials, position);
		buffer_counter--;
		printf("- Parameter 2 has changed -\n");
		printf("Material %d has been removed from input buffer\n", material);
		printf("Input Buffer: ");
		printList(&materials);
		printf("Input Buffer Size: %d\n", buffer_counter);
		printf("\n");
		return 0;
	}
	
	else
	{
		return -1;
	}
}


void insert_tool(int tool)
{
	appendNode(&tools, tool);
	tools_counter++;
}


int remove_tool(int index)
{
	
	if (tools_counter > 0)
	{
		removeNode(&tools, index);
		tools_counter--;
		return 0;
	}
	
	else
	{
		return -1;
	}
}


void insert_product(int product)
{
	char pro = ' ';
	appendNode(&output_queue, product);
	output_queue_counter++;
	if (product == 111)
	{
		productA_counter++;
		pro = 'A';
	}
	else if (product == 222)
	{
		productB_counter++;
		pro = 'B';
	}
	else if (product == 333)
	{
		productC_counter++;
		pro = 'C';
	}
	printf("- Parameter 3 has changed -\n");
	printf("Product %c has been produced\n", pro);
	printf("Product A: %d produced\n", productA_counter);
	printf("Product B: %d produced\n", productB_counter);
	printf("Product C: %d produced\n", productC_counter);
	printf("\n");
	printf("- Parameter 4 has changed -\n");
	printf("Output Queue: ");
	printOutputQueue(&output_queue);
	printf("Output Queue Size: %d\n", output_queue_counter);
	printf("\n");
}


void *generator_producer(void *param)
{
	
	int material = *(int *) param;

	
	while (1)
	{
		
		pthread_mutex_lock(&mutex);

		if (initial_grab == 1 && buffer_counter < MAX_BUFFER_SIZE)
		{
			if (material == 1)
			{
				
				if (material1_in_buffer - material2_in_buffer < 3 && material1_in_buffer - material3_in_buffer < 3
					&& material1_counter - material2_counter < 3 && material1_counter - material3_counter < 3)
				{
					if (insert_material(material) == 0)
					{
						material1_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 1 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 2)
			{
				
				if (material2_in_buffer - material1_in_buffer < 3 && material2_in_buffer - material3_in_buffer < 3
					&& material2_counter - material1_counter < 3 && material2_counter - material3_counter < 3)
				{
					if (insert_material(material) == 0)
					{
						material2_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 2 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 3)
			{
				
				if (material3_in_buffer - material1_in_buffer < 3 && material3_in_buffer - material2_counter < 3
					&& material3_counter - material1_counter < 3 && material3_counter - material2_counter < 3)
				{
					if (insert_material(material))
					{
						material3_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 3 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
		}
		
		else if (initial_grab == 0 && buffer_counter < MAX_BUFFER_SIZE-2)
		{
			if (material == 1)
			{
				
				if (buffer_counter == 0 
					|| (material1_in_buffer < 3 && material2_in_buffer > 0)
					|| (material1_in_buffer < 3 && material3_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material1_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 1 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 2)
			{
				
				if (buffer_counter == 0 
					|| (material2_in_buffer < 3 && material1_in_buffer > 0)
					|| (material2_in_buffer < 3 && material3_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material2_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 2 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 3)
			{
				
				if (buffer_counter == 0 
					|| (material3_in_buffer < 3 && material1_in_buffer > 0)
					|| (material3_in_buffer < 3 && material2_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material3_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 3 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
		}
		
		pthread_mutex_unlock(&mutex);
	}
}


void *operator_consumer_materials(void *param)
{
	
	struct operator_struct *operator = (struct operator_struct *) param;	
	srand(time(NULL));
	int material_position, material;

	
	while (1)
	{
		
		pthread_mutex_lock(&mutex);

		
		if (initial_grab == 1)
		{
			if (buffer_counter == MAX_BUFFER_SIZE)
			{
				
				material_position = rand() % buffer_counter;
				material = getNodeData(&materials, material_position);
				
				while (material == operator->operator_materials[0])
				{
					material_position = rand() % buffer_counter;
					material = getNodeData(&materials, material_position);
				}

				remove_material(material_position);
				operator->operator_materials[0] = material;

				initial_grab = 0;
			}
		}
		else if (buffer_counter > 0 && (operator->operator_materials[0] == 0 || operator->operator_materials[1] == 0))
		{
			
			if (buffer_counter > 0)
			{
				
				material_position = rand() % buffer_counter;
				material = getNodeData(&materials, material_position);
				
				while (material == operator->operator_materials[0])
				{
					material_position = rand() % buffer_counter;
					material = getNodeData(&materials, material_position);
				}

				
                remove_material(material_position);
                
                if (operator->operator_materials[0] == 0)
                    operator->operator_materials[0] = material;
                else
                    operator->operator_materials[1] = material;
			}
		}
		
		pthread_mutex_unlock(&mutex);
	}
}


void *operator_consumer_tools(void *param)
{
	
	struct operator_struct *operator = (struct operator_struct *) param;	
	srand(time(NULL));
	int tool_position1, tool_position2, tool1, tool2;

	
	while (1)
	{
		
		pthread_mutex_lock(&mutex);

		
		if (tools_counter > 1 && operator->operator_tools[0] == 0 && operator->operator_tools[1] == 0)
		{
			
			tool_position1 = rand() % tools_counter;
			tool1 = getNodeData(&tools, tool_position1);
			remove_tool(tool_position1);
			operator->operator_tools[0] = tool1;

			
			tool_position2 = rand() % tools_counter;
			tool2 = getNodeData(&tools, tool_position2);
			
			while (tool2 == tool1)
			{
				tool_position2 = rand() % tools_counter;
				tool2 = getNodeData(&tools, tool_position2);
			}
			remove_tool(tool_position2);
			operator->operator_tools[1] = tool2;

		}

		
		pthread_mutex_unlock(&mutex);
	}
}


void *operator_producer(void *param)
{
	
	struct operator_struct *operator = (struct operator_struct *) param;
	srand(time(NULL));
	int product;
	float random_time;

	
	while (1)
	{
		
		random_time = ((float)rand()) / RAND_MAX;
		sleep(random_time);

		product = 0;
		
		
		pthread_mutex_lock(&mutex);

		
		if (operator->operator_materials[0] != 0 && operator->operator_materials[1] != 0 && operator->operator_tools[0] != 0 && operator->operator_tools[1] != 0)
		{
			
			if ( ((operator->operator_materials[0] == 1 && operator->operator_materials[1] == 2) || (operator->operator_materials[0] == 2 && operator->operator_materials[1] == 1)) 
				&& ((operator->operator_tools[0] == 11 && operator->operator_tools[1] == 22) || (operator->operator_tools[0] == 22 && operator->operator_tools[1] == 11)) )
			{
				
				if (productA_counter - productB_counter < 9 && productA_counter - productC_counter < 9)
					product = 111;
				else
				{
					
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    
                    if (insert_material(operator->operator_materials[0]) == 0)
					operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
					operator->operator_materials[1] = 0;
				}
			}
			
			else if ( ((operator->operator_materials[0] == 2 && operator->operator_materials[1] == 3) || (operator->operator_materials[0] == 3 && operator->operator_materials[1] == 2)) 
				&& ((operator->operator_tools[0] == 22 && operator->operator_tools[1] == 33) || (operator->operator_tools[0] == 33 && operator->operator_tools[1] == 22)) )
			{
				
				if (productB_counter - productA_counter < 9 && productB_counter - productC_counter < 9)
					product = 222;
				else
				{
					
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
			}
			
			else if ( ((operator->operator_materials[0] == 3 && operator->operator_materials[1] == 1) || (operator->operator_materials[0] == 1 && operator->operator_materials[1] == 3)) 
				&& ((operator->operator_tools[0] == 33 && operator->operator_tools[1] == 11) || (operator->operator_tools[0] == 11 && operator->operator_tools[1] == 33)) )
			{
				
				if (productC_counter - productA_counter < 9 && productC_counter - productB_counter < 9)
					product = 333;
				else
				{
					
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
			}
			
			else
			{
				
				insert_tool(operator->operator_tools[0]);
				insert_tool(operator->operator_tools[1]);
				operator->operator_tools[0] = 0;
				operator->operator_tools[1] = 0;
				
				if (insert_material(operator->operator_materials[0]) == 0)
					operator->operator_materials[0] = 0;
				if (insert_material(operator->operator_materials[1]) == 0)
					operator->operator_materials[1] = 0;
			}

			
			if (product == 111 || product == 222 || product == 333)
			{
				
				if (output_queue_counter > 0 && getNodeData(&output_queue, output_queue_counter-1) == product)
				{
					
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
				else
				{
					insert_product(product);
					
					operator->operator_materials[0] = 0;
					operator->operator_materials[1] = 0;
					
					insert_tool(operator->operator_tools[0]);
					insert_tool(operator->operator_tools[1]);
					operator->operator_tools[0] = 0;
					operator->operator_tools[1] = 0;
				}
			}
		}
		
		pthread_mutex_unlock(&mutex);
	}
}


int getch(void)
{
        int c;
        struct termios old_attr, new_attr;

		tcgetattr(STDIN_FILENO, &old_attr);
		
        new_attr = old_attr;
		new_attr.c_lflag &= ~(ICANON|ECHO);
		
		tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
		
		c = getchar();
		
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
        
        return c;
}


void *pause_resume(void *param)
{
	int isPaused = 0;
	char key;

	
	while (1)
	{
		key = getch();
		
		if (key == 'p')
		{
			if (isPaused == 0)
			{
				pthread_mutex_lock(&mutex);
				printf("\n\n--- PROGRAM PAUSED ---\n");
				printf("\nStatus of current parameters:\n\n");
				printf("Parameter 1.\nMaterial 1: %d generated\n", material1_counter);
				printf("Material 2: %d generated\n", material2_counter);
				printf("Material 3: %d generated\n\n", material3_counter);
				printf("Parameter 2.\nInput Buffer: ");
				printList(&materials);
				printf("Input Buffer Size: %d\n\n", buffer_counter);
				printf("Parameter 3.\nProduct A: %d produced\n", productA_counter);
				printf("Product B: %d produced\n", productB_counter);
				printf("Product C: %d produced\n\n", productC_counter);
				printf("Parameter 4.\nOutput Queue: ");
				printOutputQueue(&output_queue);
				printf("Output Queue Size: %d\n", output_queue_counter);
				printf("\n\nEnter 'r' to resume the program.\n");

				isPaused = 1;
			}
		}
		
		else if (key == 'r')
		{
			isPaused = 0;
			pthread_mutex_unlock(&mutex);
		}
	}
}


int main(int argc, char *argv[])
{
	
	insert_tool(11);
	insert_tool(22);
	insert_tool(33);

	
	pthread_t tid;	

	
	int material1 = 1;
	int material2 = 2;
	int material3 = 3;
	pthread_create(&tid, NULL, generator_producer, &material1);
	pthread_create(&tid, NULL, generator_producer, &material2);
	pthread_create(&tid, NULL, generator_producer, &material3);

	
    struct operator_struct operator1;
    operator1.operator_materials[0] = 0;
    operator1.operator_materials[1] = 0;
    operator1.operator_tools[0] = 0;
    operator1.operator_tools[1] = 0;

    struct operator_struct operator2;
    operator2.operator_materials[0] = 0;
    operator2.operator_materials[1] = 0;
    operator2.operator_tools[0] = 0;
    operator2.operator_tools[1] = 0;

    struct operator_struct operator3;
    operator3.operator_materials[0] = 0;
    operator3.operator_materials[1] = 0;
    operator3.operator_tools[0] = 0;
	operator3.operator_tools[1] = 0;

    
	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator1);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator1);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator1);

	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator2);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator2);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator2);

	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator3);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator3);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator3);

	
	pthread_create(&tid, NULL, pause_resume, NULL);

	
	pthread_join(tid, NULL);

	return 0;
}
