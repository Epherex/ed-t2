#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_util.h"
#include "commands.h"
#include "modules/sig/object.h"
#include "modules/data_structures/static_list.h"

int main(int argc, char *argv[]) {
	StList objList = StList_Create(1000);

	char *baseDir = NULL;
	char *entryFileName = NULL;
	char *queryFileName = NULL;
	char *outputDir = NULL;
	char *outputSVGFileName = NULL;

	FILE *entryFile = NULL;
	FILE *queryFile = NULL;
	FILE *outputSVGFile = NULL;
	FILE *outputQrySVGFile = NULL;
	FILE *outputTXTFile = NULL;

	// Processamento dos argumentos passados ao programa
	for(int i = 1; i < argc; i++) {
		if(strcmp("-e", argv[i]) == 0) {
			if(++i >= argc) {
				printf("O argumento '-e' requer um diretório!\n");
				return 1;
			}
			if(baseDir != NULL) {
				free(baseDir);
			}
			baseDir = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(baseDir, argv[i]);
		} else if(strcmp("-f", argv[i]) == 0) {
			if(++i >= argc) {
				printf("O argumento '-f' requer o nome de um arquivo!\n");
				return 1;
			}
			if(entryFileName != NULL) {
				free(entryFileName);
			}
			entryFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(entryFileName, argv[i]);
		} else if(strcmp("-q", argv[i]) == 0) {
			if(++i >= argc) {
				printf("O argumento '-q' requer o nome de um arquivo!\n");
				return 1;
			}
			if(queryFileName != NULL) {
				free(queryFileName);
			}
			queryFileName = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(queryFileName, argv[i]);
		} else if(strcmp("-o", argv[i]) == 0) {
			if(++i >= argc) {
				printf("O argumento '-o' requer um diretório!\n");
				return 1;
			}
			if(outputDir != NULL) {
				free(outputDir);
			}
			outputDir = malloc((strlen(argv[i]) + 1) * sizeof(char));
			strcpy(outputDir, argv[i]);
		} else {
			printf("Comando não reconhecido: '%s'\n", argv[i]);
			return 1;
		}
	}

	// Verificação se os argumentos obrigatórios foram passados
	if(entryFileName == NULL) {
		printf("O argumento '-f' é obrigatório!\n");
		return 1;
	}
	if(outputDir == NULL) {
		printf("O argumento '-o' é obrigatório!\n");
		return 1;
	}

	outputSVGFileName = malloc((strlen(entryFileName) + 4) * sizeof(char));
	strcpy(outputSVGFileName, entryFileName);
	changeExtension(outputSVGFileName, "svg");

	// Abertura do arquivo de entrada padrão
	entryFile = openFile(baseDir, entryFileName, "r");
	if(entryFile == NULL) {
		return 1;
	}

	// Abertura dos arquivos referentes à consulta
	char outputTXTFileName[64];
	char outputQrySVGFileName[64];
	if(queryFileName != NULL) {	
		queryFile = openFile(baseDir, queryFileName, "r");
		if(queryFile == NULL)
			return 1;
		
		char noExtName[32];
		// Copiar nome completo do arquivo de consulta
		strcpy(noExtName, queryFileName);
		// Remover diretório e extensão
		removeDirAndExt(noExtName);

		// Copiar nome do arquivo de entrada
		strcpy(outputTXTFileName, entryFileName);
		// Adicionar sufixo (que é o nome do arquivo de consulta)
		addSuffix(outputTXTFileName, noExtName);
		// Copiar o resultado para o arquivo SVG de consulta
		strcpy(outputQrySVGFileName, outputTXTFileName);

		// Atribuir as extensões correspondentes
		changeExtension(outputTXTFileName, "txt");
		changeExtension(outputQrySVGFileName, "svg");

		outputTXTFile = openFile(outputDir, outputTXTFileName, "w");
		if(outputTXTFile == NULL)
			return 1;

		outputQrySVGFile = openFile(outputDir, outputQrySVGFileName, "w");
		if(outputQrySVGFile == NULL)
			return 1;
	}

	// Abertura do arquivo de saída padrão
	outputSVGFile = openFile(outputDir, outputSVGFileName, "w");
	if(outputSVGFile == NULL) {
		return 1;
	}

	// Processar comandos do .geo
	if(!processGeometry(entryFile, outputSVGFile, outputQrySVGFile, objList))
		return 1;

	// Processar comandos do .qry, se houver
	if(queryFile != NULL)
		if(!processQuery(queryFile, outputQrySVGFile, outputTXTFile, objList, outputDir, outputQrySVGFileName))
			return 1;
	
	// Limpeza
	fclose(entryFile);
	fclose(outputSVGFile);
	if(queryFile != NULL) {
		fclose(queryFile);
		fclose(outputQrySVGFile);
		fclose(outputTXTFile);
		free(queryFileName);
	}
	if(baseDir != NULL)
		free(baseDir);
	free(outputSVGFileName);
	free(entryFileName);
	if(outputDir != NULL)
		free(outputDir);
	StList_Destroy(objList, Object_Destroy);
}