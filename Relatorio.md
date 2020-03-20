#Relatorio EP0
##Alexandre Muller Jones - nUSP 8038149

##Estrutura geral
Por conta da complexidade e do tipo de problema que estamos modelando, avaliei que seriam necessários 3 arquivos principais para uma boa claridade e organização do código: um arquivo com a classe Aviao, que modela cada avião que entra em contato com o aeroporto, um com a classe Fila, que cuida da organização desses aviões, e o arquivo principal que contem o código para a simulação. Abaixo, separarei o relatório em 3 sessões, uma para cada arquivo.

##aviao.h
Esta classe contem todas as informações relacionadas a um avião, assim como métodos para as manipular. Temos as informações do código do voo, o aeroporto de origem ou destino do avião, valores para seu combustivel, tempo de voo, tempo em espera e flags que marcam se é um voo de emergência e se é um pouso (caso contrario é uma decolagem), todas as informações básicas. Temos também alguns outros valores, utilizados para organização e para os algoritmos do main.

###Valores auxiliares
Dentre estes, temos uma flag que marca se um voo é ou não "prioridade". Meu programa utiliza essa flag para marcar voos que precisam ser movidos mais para o inicio da fila, o que inclui voos de emergência assim como voos com pouco combustivel sobrando (escolhi 20% do máximo de combustivel que um avião pode ter como base de comparação) e voos em espera há muito tempo.

Temos também 2 ponteiros para outros Aviões, usados pela classe Fila, e uma flag foiMovido, usada pelo algoritmo que reorganiza a fila para garantir que cada avião seja movido apenas uma vez.

###*init()*
Este método popula os campos do voo, basicamente criando as informações sobre ele. Para isso, temos, no inicio do arquivo, duas listas de strings. A primeira contem as siglas de 5 empresas aéreas imaginárias, com 2 letras cada, enquanto a segunda contem os códigos de 30 aeroportos. Utilizei 29 dos maiores aeroportos do mundo e nosso próprio aeroporto de Guarulhos, GRU, para montar essa lista.

Essas 2 listas são usadas junto de um gerador de numeros aleatórios para sortear um código, a partir de uma das 5 empresas e 3 digitos sorteados aleatóriamente, assim como sua origem e destino, se é um pouso ou uma decolagem, qual o combustivel inicial ou tempo de voo esperado, etc. Selecionamos aqui, também, a frequencia com a qual temos voos de emergencia, escolhida como 1:20 (1 a cada 20 voos são emergências). Além disso, setamos todas as flags auxiliares para seus valores iniciais.

###Outros metodos
Temos também 2 outros métodos, um que verifica se o voo em questão é prioritário conforme os requisitos listados acima e outro que verifica se é um voo emergencial.

##fila.h
Este arquivo contem a classe que cuida de organizar a estrutura da fila. Implementei esta fila por meio de uma lista duplamente ligada, guardando sempre o endereço tanto do primeiro avião da fila como do último para maior velocidade em operações de manipulação dela, e também o número total deles.

Essa classe tem 3 getters, que buscam o primeiro avião, o último e o numero total deles, assim como outros 5 métodos de manipulação.

###*remove()*
Este método remove da fila um avião apontado por av. Na prática, isto significa arrumar os ponteiros dos avioes ao redor, decrescer o total de avioes na fila e marcar os apontadores do avião para nullptr. Este método é utilizado em qualquer manipulação feita sobre a fila de modo a garantir que não haja nenhum problema.

###*insereFim()*
Este metodo insere o aviao que é passado como parametro no fim da lista, arrumando os ponteiros e incrementando o total. É utilizado no início de toda etapa de simulação quando são gerados os novos voos que entraram em contato naquele instante, que são sempre inseridos no fim da fila.

###*insereIni()*
Similar ao anterior, com a diferença de estar inserindo no inicio ao invés do final. É utilizado sempre que é preciso trazer mais para o início voos prioritários ou de emergência.

###*insereDepois()*
Método utilizado para inserções em posições especificas. Na simulação, é usado para que todo voo marcado como prioritário que não seja emergencial seja inserido após todos os voos emergenciais, o que garante que estes serão sempre os primeiros da fila.

###*inserePrimeiro()*
Metodo utilizado apenas para o primeiro voo gerado no inicio da simulação, dado que o procedimento para sua inserção é diferente do procedimento quando a fila ja tem outros membros.

##main.cpp
Arquivo com o código principal da simulação. As variáveis principais utilizadas tem comentários no próprio código explicando uma a uma. Aqui, darei um panorama geral do passo a passo do programa.

###Leitura de valores
O programa tem 2 formas de receber os paramentros de simulação: inserir um a um manualmente ou passá-los como argumentos na linha de comando. O programa tenta ler os valores diretamente de lá, conferindo se há exatamente 5 argumentos (K, V, C, T e a semente do gerador de números aleatórios, nessa ordem). Caso contrário, o programa printa no terminal o número de argumentos recebidos para conferência e realiza o processo de leitura manual dos paramentros.

O gerador de números aleatórios é inicializado com a semente passada e se inicia a simulação.

###Simulação
####Novos voos
A cada etapa, é gerada uma leva de até K novos aviões entrando em contato com o aeroporto. Estes são inseridos no fim da fila e mostrados na saída. Em seguida, todos os aviões da fila tem suas flags foiMovido resetadas, dado que essa flag muda a cada iteração.

####Prioridades
Após isso, se verifica a prioridade de cada voo da fila e reposiciona nela os voos prioritários. Voos emergenciais são colocados bem no início, seguidos de voos prioritarios não-emergenciais, seguidos então do resto da fila em ordem cronológica de contato com a torre. A flag foiMovido é setada sempre que um voo tem sua posição na fila alterada, de forma que isso ocorra apenas uma vez e a implementação da iteração pela lista seja mais simples. Sempre que um voo precisa ser movido, ele é removido e depois inserido na posição certa.

####Encaminhamento para pistas
O próximo passo é esvaziar a fila o quanto seja possivel, direcionando voos para as pistas corretas. Cada pista é representada por um int equivalente a quanto tempo a pista precisa para estar disponível. Se esse valor é 0, a pista está disponível, portanto é direcionado à ela o primeiro voo da fila e ela recebe o valor 2. Caso a pista não esteja disponível (qualquer valor diferente de 0), o valor nela é decrementado, ou seja, depois de utilizada a pista passa 2 instantes indísponivel conforme a especificação de segurança.

As características especiais da pista 3, de estar disponível apenas para decolagem com excessão de voos de emergência, foram inseridas manualmente no código. 

####Lidando com emergencias
Depois que esse processo acaba, verificamos se há algum voo emergencial sobrando na fila, e estes são redirecionados para outros aeroportos conforme a especificação. Cada ocorrência desta condição é contabilizada nas estatísticas da simulação.

Busquei enviar para a saída todas as ocorrencias principais do aeroporto, incluindo todos os novos voos, a situação da fila a cada etapa da simulação, para qual pista cada voo removido da fila foi direcionado e que tipo de voo era, se voos foram redirecionados por emergencia etc.

####Calculo de medias
A cada etapa da simulação e em alguns momentos relevantes, são captadas informações utilizadas no calculo das médias pedidas. São contados quantos aviões pousam e quanto combustível cada um destes tem sobrando, possibilitanto assim calcular a média móvel deste valor ao longo da simulação. Conto quantos aviões decolam e somamos o tempo que cada um esperou pela decolagem, calculando esta outra media também.

Itero também pela lista somando o total de combustivel disponivel entre todos os aviões da fila, que, em conjunto com o total de aviões nela, permite calcular mais uma das medias. As outras médias e valores pedidos tem sua contagem e cálculo bastante direto e apontado por comentários no próprio código.

Para melhor leitura e acompanhamento da simulação, ao fim de cada etapa, o programa pausa e espera que o usuario pressione *enter* para voltar ao início e executar a proxima etapa. Este passo foi incluido de modo que, durante tanto o processo de depuração como o processo de avaliação, seja possível verificar com calma cada valor e avaliar o estado da simulação antes de progredir para mais uma iteração.

###Liberação de memória
Ao fim da simulação, a fila é esvaziada e a memória usada por cada avião é liberada, de modo a não ter nenhum vazamento.

###Funções auxiliares
####entradaManual()
Le manualmente os valores dos paramentros da simulação um a um.

####geraNovos()
Cria e insere na fila os novos voos que entram em contato a cada iteração.

####pulaLinha()
Printa uma linha em branco na saida. Função usada apenas para melhorar a leitura da saida do programa.

####listaAvioes()
Imprime a lista de aviões em espera na fila por código e listando se cada um é decolagem ou pouso. Utilizada também para melhor acompanhamento da evolução da simulação.

##Considerações finais
Apesar de ser um problema aparentemente simples, a implementação das soluções para ele foi interessante. Tive que considerar casos de borda inexperados, assim como criar soluções que fossem tanto eficientes como de facil leitura para problemas mais diretos.
