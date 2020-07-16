/* Data de submissão:
 * Nome: Nicolas de Sousa Imagawa
 * RA: 204147
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Funcao para verificar se um numero eh primo. Devido aos numeros estarem entre 0 e 2⁽^(64)-1, a funcao trabalha com uma entrada long int.

long int check_prime(long int input)
{
	//Counter eh o contador do loop
        int counter;
	//mult serve para detectar quantos multiplos a entrada tem
        int mult = 0;
	//result eh a variavel que a funcao retorna, sendo 0 para nao-primos e 1 para primos.
        int result = 0;
	
	//Calcula-se o resto da entrada por todos os numeros ate ele.
        for(counter = 1; counter<=input; counter++)
        {	
		//Em caso do resto ser 0, mult eh incrementado.	
                if((input % counter) == 0)
                {
                        mult = mult + 1;
                }
        }
	
	//Se o resto foi 0 mais de duas vezes, o numero nao eh primo.
        if(mult > 2)
        {
                result = 0;
        }

	//Caso contrario, o numero eh primo.
        else
        {
                result = 1;
        }

	//Se o valor de entrada for 0 ou 1, tambem retornamos 0.
	if(input == 0 || input == 1)
        {
                result = 0;
        }

        return result;
}

//int count = 0;

int main () {

	//variavel para o pid do processo filho.
	pid_t childpid;
	//Result recebe o valor da funcao check_prime
    	int result;
	
	//max se refere ao numero maximo de processos filhos que podem ser executados.
	int max;
	
	//Ponteiro para o numero de primos que ha na cadeia de entrada
	long int *n_of_primes;	
	
	//Variaveis para o vetor de ints usado na conversao da string para int,tamanho da string e cursor que percorre a string, respectivamente.
	long int input_num[1000], length = 0, cursor;
	
	//Character se refere ao valor a ser escrito como long int no vetor input_num, ou seja, para ser processado como numero.
	long int character;
	//Input eh a string de entrada inicial
	char input[1000];

	//Le-se a string de entrada ate um fim de linha.
	fgets(input, sizeof(input), stdin);

	//Como não podemos incrementar a string de entrada em si, craimos um ponteiro para a string.
	char* char_input = input;

	//Numero de processos em execucao. Inicializado em 0 para evitar lixo.
	int process = 0;
	
	//Status se refere a situacao do processo.
	int status;
	//i eh um contador generico para ser usado nos loops da main.	
	int i = 0;

	//Flags para a permissao de compartilhar dados entre processos.
	int protection = PROT_READ | PROT_WRITE;
 	int visibility = MAP_SHARED | MAP_ANON;
	
	//Loop responsavel por ler a string de entrada e converte-la em long ints. cada posicao do vetor de long ints recebe um valor percorrido pelo incremento de char_input, sendo esse carregado pelo %ln vinculado a cursor.
	while (sscanf(char_input, "%ld%ln", &character, &cursor) > 0)
	{
    	input_num[length++] = character;
    	char_input += cursor;
	}	
	
	//Numero maximo de processos.
	max = 4;
	
	//Configuracao do compartilhamento de dados entre processos.
  	n_of_primes = (long int *)mmap(NULL, sizeof(long int), protection, visibility, 0, 0);	
	//Inicializacao do conteudo do ponteiro em 0.
	(*n_of_primes) = 0;
	
	//loop responsavel por verificar numero a numero do vetor, respeitando o maximo de 4 processos.
	for(int i = 0; i < length; i++)
	{
		//Incremento dos processos ate que se atinja o limite.
		process = process + 1;
		
		while(process >= max)
		{
			//Caso o numero de processos seja 4, esperamos a saida de um deles para dar procedimento ao programa.
			wait(&status);
			process = process - 1;
		}	
		
		//Criacao de um processo filho.
		childpid = fork();
		
		//Se o pid do processo eh 0, checamos se o numero em questao eh primo.
		if(childpid == 0)
		{
			result = check_prime(input_num[i]);
			if(result == 1)
			{
				(*n_of_primes) = (*n_of_primes) + 1;
			}
			exit(0);			
		}
	}
	//Finalizamos os processos antes de terminar o programa.
	while ((wait(&status) > 0));
	
	//print do total de numeros primos da string de entrada.
	printf("%ld\n", *n_of_primes);
   	return 0;	
}
