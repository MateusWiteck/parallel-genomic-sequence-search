# Instruções para usar o Makefile

Por padrão, o Makefile já busca os arquivos em seus devidos diretórios, e adota o formato `arquivo1-*` ou `arquivo2-*` como prefixo padrão dos arquivos de teste. Assim, para um arquivo de busca `example.c` em `src/`, o comando `make example teste-1` utilizaria os arquivos `arquivo1-teste-1` e `arquivo2-teste-1` para o teste realizado, utilizando o arquivo de busca `example.c`. O `make run` pode ser utilizado para rodar novamente um arquivo já compilado, por exemplo: `make run-example teste-2`. Por padrão, o Makefile utiliza o arquivo de busca `reference/busca_cadeia.c` e os arquivos de teste `arquivo1` e `arquivo2-0`.

Obs.: Para retrocompatibilidade, o Makefile também considera o hífen como opcional (para funcionar com o `arquivo1b`/`arquivo2b`); no geral, criar arquivos de teste no formato `arquivo[1-2]-*`.
