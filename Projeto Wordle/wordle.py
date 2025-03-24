######################################################################################################################################################
# Esse projeto foi feito apenas por mim, Guilherme Roth Cassol. Para chegar nessa solução .
# Esse código possui todas funções utilizadas na resolução do jogo WORDLE para 5 letras. 
# Para rodar esse código é necessário instalar algumas dependências, como o nltk e o selenium.
# Importante ressaltar que o código não está polido e com certeza há muito oque fazer em relação de boas práticas
#
# Este é o site caso queira aprender as regras
# "https://mikhad.github.io/wordle/#infinite"
#
# A explicação a seguir resume brevemente como o código funciona, sem explicar todas otimizações e os motivos de o porque foi implementado dessa maneira,
# visando evitar um texto muito grande.
# A resolução funciona da seguinte maneira:
# - Sendo o passo zero o código usa do dicionário importado e do arquivo "adicao.txt" (palavras que não estão no dicionario mas estão no site) e passa
#   por todas elas, incrementando seu valor em um mapa de letras (chave sendo a letra), com o objetivo de saber qual letra mais se repete, assim poderemos
#   adicionar um valor para cada palavra, que seria o quão bom ela vai ser em cobrir o máximo de palavras possíveis.
# - O primeiro chute é feito com a palavra "soare", que é a palavra com o maior valor de acordo com o mapa criado.
# - O segundo passo é interpretar as informações dadas pela cor de cada letras, sem aprofundar muito no funcionamento as possiveis informações são:
# --- Lugar correto de uma letra (Verde), Lugar incorreto de uma letra (Amarela), Letra não existente (Cinza), entre outras otimizações a mais, como
#     a quantidade máxima de uma letra na palavra (quando uma palavra repete uma letra, e uma delas está cinza e outra laranja), também mostrando que 
#     a letra não está no lugar onde está cinza, mas enfim esse é apenas um exemplo das micro otimizações que compõe o algoritmo.
# - O terceiro passo é escolher a próxima palavra que será chutada, isso pode ser divido em alguns sub-passos:
# --- Primeiro todas palavras que não correspondem as informações recolhidas são retiradas do dicionário local, mantendo apenas as palavras possíveis,
#     outro detalhe é que as palavras que repetem letras tem menos prioridade.
# --- Segundo o mapa do valor das letras é refeito baseado no novo dicionário.
# --- Por último um novo dicionário é ordenado, mas esse dicionário continua possuindo todas as palavras, diferente do local que possui apenas as possíveis,
#     ordenando-o e escolhendo a palavra com maior valor baseado no mapa de letras feito no passo anterior.
# - Agora com a nova palavra escolhida o código volta para o primeiro passo, até que a palavra esteja toda verde, significando uma vitória.
#
# Obs: Outra otimização que acredito que fosse interessante, seria o quão frequente tal letra aparece em uma posição especifica, acabei não implementando,
# pois além de ser uma mão na massa a mais, o algoritmo como está já tem um score quase perfeito, raramente errando a palavra.
######################################################################################################################################################

import os
import nltk
nltk.download('words')
from nltk.corpus import words
import random

word_list = words.words()
descart_list = set()
for word in open("descarte.txt"):
    descart_list.add(word.strip())

short_words = [word.strip() for word in word_list if len(word) == 5 and word.isalpha() and not word[0].istitle() and word.strip() not in descart_list]

word_list = list(set(short_words))
for word in open("adicao.txt"):
    if word.strip() != "":
        word_list.append(word.strip())


listaOriginal = word_list.copy()

os.system("clear")

valorLetras = dict()
# Quantidade de repeticao de letras
def dicionarioRepeticao():
    for word in word_list:
        for letter in word:
            if letter not in valorLetras:
                valorLetras[letter] = 0
            valorLetras[letter] += 1
dicionarioRepeticao()

def verificarRepeticao(palavra):
    for i in range(len(palavra)):
        if palavra[i] in palavra[(i+1):]:
            return True
    return False

def valorPalavra(palavra):
    valorTotal = 0
    if verificarRepeticao(palavra):
        return 0
    for letra in palavra:
        if letra in valorLetras:
            valorTotal += valorLetras[letra]
    return -valorTotal

def valorPalavraSemRepeticao(palavra):
    valorTotal = 0
    for letra in palavra:
        if letra in valorLetras:
            valorTotal += valorLetras[letra]
    return -valorTotal

def melhorChute():
    for palavra in listaOriginal:
        melhorPalavra = True
        for letra in palavra:
            if letra in posicaoCorreta.values() or letra in letraCorreta:
                melhorPalavra = False
                break
        if melhorPalavra:
            return palavra

word_list.sort()
word_list = sorted(word_list, key=valorPalavra)

# Chave index, Conteudo letra
posicaoCorreta = dict()

# Chave letra, Conteudo quantidade de aparicoes
letraCorreta = dict()

# Chave index, Conteudo lista de letras que nao estao ali
aliNao = dict()

# Caso esteja preenchido significa que o numero de letras eh aquele 100%
quantAbsoluta = set()

# Palavra com maior quantidade de letras comumm sem repeticoes
firstGuess = "arose"

# Lista de palavras que nao tem na lista
palavrasIncorretas = []

# Lista de palavras a serem adicionadas
palavrasAdicionadas = []

# Lista de possíveis palavras
possiveisPalavras = []

def dicionarioRepeticaoQuatro():
    for word in word_list:
        for letter in word:
            if letter not in valorLetras:
                valorLetras[letter] = 0
            if letter not in letraCorreta and letter not in posicaoCorreta.values():
            	valorLetras[letter] += 1

# - (#) Nao tem
# - ($) Posicao errada
# - (@) Posicao certa

def parseLinha(linha):
	quantLetra = dict()
	for i in range(0, len(linha), 2):
		if linha[i] == "$":
			if linha[i+1] not in quantLetra:
				quantLetra[linha[i+1]] = 0
			quantLetra[linha[i+1]] += 1
   
			if (i+1)//2 not in aliNao:
				aliNao[(i+1)//2] = []
			aliNao[(i+1)//2].append(linha[i+1])
   
		if linha[i] == "@":
			posicaoCorreta[(i+1)//2] = linha[i+1]
			if linha[i+1] not in quantLetra:
				quantLetra[linha[i+1]] = 0
			quantLetra[linha[i+1]] += 1
               
	for letra in quantLetra:
		if letra in letraCorreta:
			if quantLetra[letra] <= letraCorreta[letra]:
				continue
		letraCorreta[letra] = quantLetra[letra]
          
	for i in range(0, len(linha), 2):
		if linha[i]	== "#":
			if linha[i+1] not in letraCorreta:
				letraCorreta[linha[i+1]] = 0
				quantAbsoluta.add(linha[i+1])
			elif letraCorreta[linha[i+1]] != 0:
				if (i+1)//2 not in aliNao:
					aliNao[(i+1)//2] = []
				aliNao[(i+1)//2].append(linha[i+1])
				quantAbsoluta.add(linha[i+1])

def checarPalavra(palavra):
    for index in posicaoCorreta:
        if palavra[index] != posicaoCorreta[index]:
            return False

    # Chave letra, Conteudo quantidade de aparicoes
    letrasAtuais = dict()
    i = 0
    for letra in palavra:
        if letra not in letrasAtuais:
            letrasAtuais[letra] = 0
        letrasAtuais[letra] += 1
        if i in aliNao:
            if letra in aliNao[i]:
                return False
        i += 1

    for letra in letraCorreta:
        if letra in letrasAtuais:
            if letra in quantAbsoluta:
                if letrasAtuais[letra] != letraCorreta[letra]:
                    return False
            elif letrasAtuais[letra] < letraCorreta[letra]:
                return False
        else:
            if letraCorreta[letra] == 0:
                continue
            return False
    return True

def printInfos():
	print("Posicoes Corretas:")
	for index in posicaoCorreta:
		print(f"Index: {index} / Palavra: {posicaoCorreta[index]}")
	print("Numero de Letras:")
	for letra in letraCorreta:
		print(f"Letra: {letra} / Quantidade: {letraCorreta[letra]}")
	print("Ali nao ta:")
	for index in aliNao:
		print(f"Index: {index} ", end="")
		for letra in aliNao[index]:
			print(f"Letra: {letra}",end="")
		print("")
	print("Quant Absoluta:")
	for letra in quantAbsoluta:
		print(f"Letra: {letra}")
          
indexAtual = 0
def palavraIncorreta(palavra):
	global indexAtual, palavrasIncorretas
	palavrasIncorretas.append(palavra)
	indexAtual += 1	

def palavraAdicional(palavra):
    global palavrasAdicionadas
    palavrasAdicionadas.append(palavra)

chuteAtual = 0   
def resetAll():
	global word_list, listaOriginal, posicaoCorreta, letraCorreta, aliNao, quantAbsoluta, palavrasIncorretas, possiveisPalavras, valorLetras, indexAtual, chuteAtual, row, palavrasAdicionadas
	with open("descarte.txt", "a") as file:
		for palavra in palavrasIncorretas:
			file.write("\n" + palavra)
	palavrasIncorretas.clear()
	with open("adicao.txt", "a") as file:
		for palavra in palavrasAdicionadas:
			file.write("\n" + palavra)
	palavrasAdicionadas.clear()
	word_list = listaOriginal
	valorLetras.clear()
	word_list.sort()
	word_list = sorted(word_list, key=valorPalavra)
	posicaoCorreta.clear()
	letraCorreta.clear()
	aliNao.clear()
	quantAbsoluta.clear()
	possiveisPalavras.clear()
	indexAtual = 0      
	chuteAtual = 0    
	row = 0

row = 0
   
def chuteQuatro():
    global listaOriginal,indexAtual, row, chuteAtual, word_list
    valorLetras.clear()
    dicionarioRepeticaoQuatro()
    listaOriginal = sorted(listaOriginal, key=valorPalavra)
    if listaOriginal:
        if len(word_list) <= 6 - row:
            if len(word_list) == 0:
                return "flago"
            aux = word_list[0]
            if aux in palavrasIncorretas:
                word_list.remove(aux)
                if len(word_list) == 0:
                    return "flago"
                aux = word_list[0]
            return aux
        return listaOriginal[indexAtual]

def teste():
    global word_list, possiveisPalavras, valorLetras, listaOriginal, indexAtual, row
    row += 1
    for palavra in word_list:
        if checarPalavra(palavra):
            possiveisPalavras.append(palavra)
    word_list = possiveisPalavras.copy()
    possiveisPalavras.clear()
    valorLetras.clear()
    dicionarioRepeticao()
    word_list = sorted(word_list, key=valorPalavra)
    listaOriginal = sorted(listaOriginal, key=valorPalavra)
    indexAtual = 0
    chuteQuatro()


# Usada para debugar

if __name__ == "__main__":        
    while True:
        escolha = input("1 - AddPosCorreta 2 - AddLetraCorreta 3 - Testar - 4 Sair 5 - AliNao 6 - Randomize 7 - QuantAbsoluta\n")
        if escolha == "1":
            index = int(input("Digite o index\n"))
            letra = input("Digite a letra\n")
            posicaoCorreta[index] = letra
        if escolha == "2":
            letra = input("Digite a letra\n")
            quantidade = int(input("Quantidade\n"))
            letraCorreta[letra] = quantidade
            if quantidade == 0:
                quantAbsoluta.add(letra)
        if escolha == "3":
            for palavra in word_list:
                if checarPalavra(palavra):
                    possiveisPalavras.append(palavra)
            word_list = possiveisPalavras.copy()
            possiveisPalavras.clear()
            valorLetras.clear()
            dicionarioRepeticao()
            word_list = sorted(word_list, key=valorPalavra)
            listaOriginal = sorted(listaOriginal, key=valorPalavra)
            printInfos()
            chuteQuatro()
        if escolha == "5":
            index = int(input("Digite o index\n"))
            letra = input("Digite a letra\n")
            if index not in aliNao:
                aliNao[index] = []
            aliNao[index].append(letra)
        if escolha == "4":
            break
        if escolha == "6":
            print(word_list[random.randint(0, len(word_list) - 1)])
            print(valorLetras)
        if escolha == "7":
            letra = input("Digite a letra\n")
            quantAbsoluta.add(letra)
        if escolha == "8":
            parseLinha(input("Digite a linha:\n"))
        if escolha == "9":
            valorLetras.clear()
            dicionarioRepeticaoQuatro()
            listaOriginal = sorted(listaOriginal, key=valorPalavra)
            if listaOriginal:
                print(listaOriginal[0])