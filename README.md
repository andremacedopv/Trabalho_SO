# Trabalho_SO
Repositório para os códigos da disciplina de Sistemas Operacionais da UnB.

## Trabalho 1 - Criação de Processos

#### Objetivo
Exercitar a criação de processos com o fork

#### Descrição
Os alunos, em grupos de 4, devem confeccionar um código em C que crie n processos (n <= 10) e imprima o pid desses processos. Cada processo deve dormir por 30 segundos e depois terminar a execução. O parâmetro n deve ser obtido na linha de comando.

#### Exemplo
```
> fork_processos 3
sou o processo pai com pid=899
sou o processo filho com pid=901
sou o processo filho com pid=902
sou o processo filho com pid=903
```

#### Entrega
Os alunos devem entregar o código fonte e, no início do código, devem fornecer (como comentário): (a) os nomes e matrículas dos integrantes do grupo; (b) a versão do SO no qual o programa foi testado; (c) a versão do compilador com o qual o programa foi compilado.

#### Observação
No trabalho, só devem ser usadas as chamadas de sistema e as bibliotecas standard do C.

## Trabalho 2 - Sincronização two-way

#### Objetivo
Exercitar a sincronização two-way com chamadas Unix

#### Descrição
Os alunos, em grupos de 4, devem confeccionar um código em C que crie 2 processos, P1 e P2. 

O processo P1 deve atualizar 20 vezes uma variável inteira em memória compartilhada (shm). Essa variável vai assumir os valores de 1 a 20,  e o valor escrito será impresso em tela a cada vez que a escrita ocorrer. O processo P2 deve ler a variável inteira em memória compartilhada (shm) e imprimir em tela o valor lido. Deve ser garantido que todo dado escrito por P1 vai ser lido por P2 uma única vez. 

Nesse programa, além do mecanismo IPC shm, podem ser utilizados os mecanismos IPC sem e/ou as primitivas de envio e recepção de sinal. O programa deve ser livre de deadlock (claro). 

#### Exemplo
```
 > le_escreve
processo <pidp1> escreveu 1
processo <pidp2> leu 1
...
processo <pidp1> escreveu 20
processo <pidp2> leu 20
```

#### Entrega
Os alunos devem entregar o código fonte e, no início do código, devem fornecer (como comentário): (a) os nomes e matrículas dos integrantes do grupo; (b) a versão do SO no qual o programa foi testado; (c) a versão do compilador com o qual o programa foi compilado.

#### Observação
No trabalho, só devem ser usadas: (a) as chamadas de sistema dos mecanismos  shm e sem e/ou de manipulação de sinais; e (b) as bibliotecas standard do C.

## Trabalho 3 - Temporizador

#### Objetivo
Exercitar temporizadores com chamadas Unix

#### Descrição
Os alunos, em grupos de 4, devem confeccionar um código em C que crie 2 processos, P1 e P2. 

O processo P1 deve executar a primitiva de recepção de mensagens do Unix por 10 vezes e, caso haja mensagem na fila, deve imprimir "mensagem <conteúdo> recebida". Caso decorram 2 segundos e não haja mensagem na fila, o processo deve imprimir "ocorreu timeout - nao recebi mensagem em 2 s" e seguir para a recepção da próxima mensagem.  Atenção: o processo não pode esperar 2 segundos e depois testar se há mensagem. Dentro do tempo permitido (2s), logo que a mensagem for enviada, ela deve ser recebida.

O processo P2 deve enviar 10 mensagens contendo o seu pid e um número de 1 a 10, em intervalos aleatórios de tempo, que vão de 1 a 4 segundos. Ao enviar a mensagem, deve imprimir "mensagem <conteúdo> enviada". As chamadas C rand e srand devem ser utilizadas para manipular números aleatórios.

Nesse programa, além do mecanismo IPC msg, podem ser utilizadas as primitivas de envio e recepção de sinal. 

#### Exemplo
```
> temporizador
nao recebi mensagem em 2s
mensagem pid=730 numero=1 enviada
recebi mensagem pid=730 numero=1
...
```

#### Entrega
Os alunos devem entregar o código fonte e, no início do código, devem fornecer (como comentário): (a) os nomes e matrículas dos integrantes do grupo; (b) a versão do SO no qual o programa foi testado; (c) a versão do compilador com o qual o programa foi compilado.

#### Observação
No trabalho, só devem ser usadas: (a) as chamadas de sistema do mecanismo msg e de manipulação de sinais; e (b) as bibliotecas standard do C.
