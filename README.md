# Projeto 1 - Computação Gráfica

## Introdução

O objetivo desse trabalho era a realização de uma aplicação interativa envolvendo os conceitos de computação gráfica e as ferramentas do OpenGL. Em primeiro momento, pensou-se em realizar um clone do jogo de Atari Space Invaders. Tal motivação norteou todo o desenvolvimento do jogo e escolha de design.

Contudo, tal objetivo não pôde ser concluído por conta da dificuldade do desenvolvedor em utilizar a linguagem nativa da biblioteca ABCg, o C++, mas especialmente por alocar tempo insuficiente para a criação desse projeto. De todo modo, algumas funções operam bem, se consideradas isoladamente.

Nesse relatório serão apresentadas a função geral de cada classe e seus relacionamentos, problemas nas implementações e um encaminhamento futuro desse projeto.

## Overview do projeto

O desenvolvimento da aplicação gira ao redor da biblioteca ABCg, desenvolvida pelo professor, e que gerencia as funções e contexto do OpenGL e outras bibliotecas adicionais. Desse modo, a primeira classe a ser desenvolvida é a openglwindow, que herda da classe homônima de ABCg. Procurou-se ao máximo delegar a essa classe somente as funcionalidades relacionada a renderização geral das imagens. Suas funções initializeGL, paintGL e terminateGL somente chamem os métodos de mesmo nome das classes componentes. paintUI gera uma interface gráfica da biblioteca Dear ImGui, e resizeGL ajusta o tamanho da tela da aplicação.

O método terminateGL tem como objetivo deletar todos os buffers e programas (glDeleteBuffer e glDeleteProgram) guardados nas propriedades das classes, então ele se assemelha em todas as classes componentes da aplicação. O mesmo pode-se dizer de initializeGL, apesar de que não tanto. initializeGL tem o objetivo de gerar os VBOs, EBOs, e VAOs necessários. Os dados usados na geração desses citados varia muito.

Em ship.cpp, initializeGL cria três VBOs, para posição e cores das linhas e faces, dois EBOs que vão ditar a ordem dos vértices que devem ser preenchidos com triângulos ou percorridos com linhas, e os VAOs guardam os dados referentes aos VBOs das faces, e o outro das linhas. Usou-se dois VAOs porque as propriedades das cores divergiam (um vértice era verde no contexto das faces da nave, mas preto em relação as linhas que contornariam as arestas). Tentou-se aproveitar ao máximo a propriedade de interpolação de cores de vértices adjacentes para criar um efeito de tridimensionalidade. Em crab.cpp, no initializeGL, o mesmo foi feito, tentando conciliar o problema das cores.

painGL de ship.cpp, além de executar a chamada de glDrawElements para adiciona a restrição ao movimento na parte inferior da tela, tentando simular o movimento do canhão de Space Invaders. paintGL também chama paintGL de bullets.cpp, que tem seu comportamento afetado pelo evento de pressionar a tecla de espaço.

Finalmente, bullets.cpp tenta implementar os tiros do canhão de Space Invaders, mas sem sucesso.

## Problemas nas soluções

Um problema nas soluções implementadas que pede por uma melhoria é a captação dos dados dos vértices dos modelos desenhados. Especialmente no caso do caranguejo, mais de 30 vértices requerem que esses dados sejam carregados de uma fonte externa e não armazenados dentro de initializeGL. Além do mais, ter os dados separados pode abrir portas, como por exemplo, o design de pixel art, armazenado em uma matriz e que posteriormente pode ser renderizado utilizando as primitivas de pontos.

Outra questão é a otimização no traçado dos contornos dos modelos. A primitiva usada, o line strip, precisa passar pelo mesmo vértice várias vezes em algumas situações, de maneira que achar uma primitiva mais adequada (ou uma combinação delas, aumentaria o desempenho).

Finalmente, uma implementação mais clara de bullets precisa ser desenvolvida, que permita disparar não somente um tiro, mas vários.

## Futuro do Projeto

Após apresentados os objetivos do projeto, possíveis soluções aos problemas e seus respectivos revezes associados, essa seção busca ser mais reflexiva.

Primeiramente, para termos um jogo funcional, precisamos concertar a dinâmica de bullets, que permita ao jogador atirar várias vezes. Além disso, criar hitbox, tanto para os inimigos quanto para o jogador, de maneira que as colisões possam ser computadas. Além disso, se quisermos ser fidedignos ao jogo de inspiração, criar os modelos da lula e do polvo, e do OVNI, bem como seus hitboxes. Por fim, implementar uma UI para que o jogador possa ver os pontos  pausar o jogo.

## Conclusão

Apesar de não ser uma aplicação completa, esse projeto conseguiu em certo nível utilizar o OpenGL de uma maneira interativa com o usuário, então ela não falhou completamente. Contudo, há muito trabalho a ser feito aqui, que deve ser levado aos próximos dias como prática.
