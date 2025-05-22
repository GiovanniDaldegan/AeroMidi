# AeroMidi
Repositório do instrumento AeroMidi, confeccionado para as matérias "Laboratório de Tecnologia Musical" e "Tópicos de Aplicação de Tecnologia à Composição Musical" no semestre 2024/2 da UnB.

Desenvolvido por:
- Amanda Andrade Oliveira Ramos
- Giovanni Daldegan
- Keila Alves da Silva Gomes

## Ideia
O instrumento foi inspirado na escaleta, de forma que o controle é feito por teclas e sopro (detectado por um microfone).

<img src="galeria/00-AeroMidi.png" width="600">

## Estrutura e Funcionamento
O instrumento consiste em uma caixa com 8 botões, cada um correspondente a uma nota da escala maior de Dó (C maior), e um tubo com um microfone na ponta. Dentro da caixa, há uma placa eletrônica (circuitos fechados com solda, jumpers e pinos para contato com outros módulos) conectada a uma placa Arduino Uno.

<p float="left">
  <img src="/galeria/01-Diagrama.png" height="265"/>
  <img src="/galeria/02-Circuito-completo.JPG" height="265"/>
</p>

O arduino é responsável por detectar a intensidade do sopro no microfone e qual tecla está sendo pressionada. Com essas medições, ele envia mensagens MIDI pela porta USB determinando a intensidade e a frequência da nota tocada. O sinal enviado chega a um computador que estiver executando o programa "atom" para escutar as mensagens MIDI e o Csound para sintetizar o som.

## Síntese sonora
A sonoridade do instrumento, sintetizada pela linguagem Csound, tenta imitar os harmônicos produzidos por uma ocarina. Utilizando a geração de ruído e síntese subtrativa do Csound, o programa produz ruído e o filtra (pass-filter) na frequência base da tecla pressionada e nos 2º, 3º, 5º e 7º harmônicos, além de gerar ondas senoidais nas mesmas frequências, para dar mais preenchimento ao som.

Para que o som gerado soe menos robótico e mecânico, as frequências das ondas senoidais sofrem uma modulação de frequência, dando uma sensação levemente mais natural ao som.
