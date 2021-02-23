# Trabalho 1.2 de SO

Projeto criado como método de avaliação da primeira unidade da disciplina de *Sistemas Operacionais* no período de 2020.2.

O projeto é capaz de calcular Integrais de duas funções utilizando a regra dos trapézios e threads.

# Informações Importantes

## Funções

As duas funções disponíveis no programa são:

- Função ID = 1:
`f(x) = 5`

- Função ID = 2:
`f(x) = sin(2*x) + cos(5*x)`

## Compilação

Compile o programa referente ao trabalho usando o Makefile.

Para a compilação, use o seguinte comando no terminal:

```shell
make
```

Para a exclusão de arquivos objetos e arquivo de execução do programa execute no terminal o comando a seguir:

```shell
make clean
```

## Instruções de uso

Para executar o programa, primeiro o compile e depois execute o arquivo gerado com o comando:

```shell
./<nome_programa> <ponto_inicial> <ponto_final> <num_trapezios> <num_threads> <id_funcao>
```

> Caso use a função dois, defina como ponto final somente o double que deve ser multiplicado pelo valor de pi.

### Exemplos de Uso

#### Função 1

Para executar o cálculo da integral da função 1 com ponto inicial igual a 0.0, ponto final igual a 10, 200 trapézios e 6 threads. Execute o comando:

```shell
./programa 0.0 10 200 6 1
```

#### Função 2

Para executar o cálculo da integral da função 2 com ponto inicial igual a 0.0, ponto final igual a 2 * pi, 200 trapézios e 7 threads. Execute o comando:

```shell
./programa 0.0 2 200 7 2
```

# Autoria

Programa desenvolvido por **Alaide Lisandra Melo Carvalho** (<mendie73@gmail.com>) e **Diego Filgueiras Balderrama** (<diegofilbal@ufrn.edu.br>) como projeto para a disciplina de *Sistemas Operacionais* do semestre de 2020.2.

&copy; IMD/UFRN 2020.