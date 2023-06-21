#include <stdio.h>
#include <string.h>

#define LIVROS "livros.dat"
#define CLIENTES "clientes.dat"
#define VENDAS "vendas.dat"
#define AUTOR "autor.dat"
#define TAM_AUTOR 3

struct reg_autor{
	int cod_autor;
	char nome_autor[200];
};
struct reg_livro{
 int codigo;
 char titulo[30];
 float preco;
 int cod_autor;
 int cod_autores[3];
};

struct reg_cliente{
  int codigo;
  char nome[40];
  char fone[20];
  char email[30];
};

struct reg_venda{
  int vendacodigo;  //Ou nro do pedido
  int vendacodcli;  //Código do Cliente que comprou 
  int vendacodliv;  //Código do Livro vendido
  int vendaqtde;    //Qtde vendida deste livro
  float vendadesc;  //Valor de desconto
  int vendasitua;
};
void cadastrarAutor(){
    struct reg_autor autor;
    FILE* fp_autor = fopen("autor.dat", "ab+"); // abre o arquivo para edição
    char op;
    int ultimo_cod_autor = 0;
    char nome_autor[200];
        
    printf("\nCadastrar AUTOR: ");
    printf("\n------------------");
    
    printf("\nNome do autor: ");
    fflush(stdin); fgets(nome_autor, sizeof(nome_autor), stdin); // nome do autor
    
    while(fread(&autor, sizeof(struct reg_autor), 1, fp_autor) == 1){ // percorre o registro dos autores
        if (autor.cod_autor > ultimo_cod_autor) { // se o cod que esta no arq autor for maior que o que será
            ultimo_cod_autor = autor.cod_autor;	// ele recebe o valor que es´ta no reg autor
        }
    }
    
    if (ultimo_cod_autor == 0) {
        ultimo_cod_autor = 1;
    } else {
        ultimo_cod_autor++; //incrementar automaticamente o id do autor
    }

    printf("\nCodigo do autor: %i", ultimo_cod_autor);
    printf("\nNome do autor: %s", nome_autor);
	
    printf("\nGravar? (S/N) ");
    fflush(stdin);
    scanf(" %c", &op);

    if ((op=='S') || (op=='s')){
        if (fp_autor == NULL) { // confirma que o arquivo auto foi aberto
            printf("\nErro ao abrir o arquivo!");
            return;
        }

        autor.cod_autor = ultimo_cod_autor;  // o cod do autor no arv autor recebe seu id
        strcpy(autor.nome_autor, nome_autor); // e copiamos a string com o nome do autor

        fseek(fp_autor, 0, SEEK_END);	// posiciona o ponteir
        fwrite(&autor, sizeof(autor), 1, fp_autor); // grava o arquivo
        printf("\nAutor Cadastrado com sucesso.");

        fclose(fp_autor);		// fecha o arquivo
    } else {
        printf("\nCadastro Cancelado!");
        return;
    }
}

void cadastrarLivro() {
    struct reg_livro livro;
    struct reg_autor autor;
    FILE* fplivro; // Variável para manipular o Arquivo de Livros
    FILE* fp_autor; // Variável para manipular o Arquivo de Autores
    char op;
    int codautor;
    int ultimo_cod_livro = 0;
    int autorEncontrado = 0; // Variável para indicar se o autor foi encontrado

    fp_autor = fopen("autor.dat", "rb"); // abre o arquivo autor
    fplivro = fopen("livros.dat", "ab+"); // abre o arquivo livros (leitura e escrita binária)
    

	
	   printf("\nCadastro de Livro");
    printf("\n---------------------------");	
	
    /// Encontrar o último código de livro cadastrado
livro.codigo = 0;
while (fread(&livro, sizeof(struct reg_livro), 1, fplivro) == 1) { // lê os registros do livro
    if (livro.codigo >= ultimo_cod_livro) {
    		ultimo_cod_livro = ultimo_cod_livro + 1; // incrementa o cod do livro
        livro.codigo = ultimo_cod_livro;
    }
}

printf("\nCodigo do Livro: %i", ultimo_cod_livro); 
	
    printf("\nTitulo: ");
    fflush(stdin);
    gets(livro.titulo);

    printf("Preco: ");
    fflush(stdin);
    scanf("%f", &livro.preco);

    printf("Codigo do autor? "); // entrada do código do autor
    scanf("%i", &livro.cod_autores[0]); // armazena o código no vetor que está no arquivo livros.dat

    while (fread(&autor, sizeof(autor), 1, fp_autor) == 1) { // percorre o arquivo autor
        if (autor.cod_autor == livro.cod_autores[0]) { // confirma se o autor que está no arquivo autor é o mesmo informado
            printf("O autor selecionado e: %s\n", autor.nome_autor);
            autorEncontrado = 1; // Marca o autor como encontrado
            break; // Sai do loop assim que o autor for encontrado
        }
    }

    if (autorEncontrado) {
        printf("Deseja adicionar mais autores para o livro? (S/N): ");
        fflush(stdin);
        scanf(" %c", &op);
        int num_autores = 1; // contador de autores adicionados

        while ((op == 'S' || op == 's') && num_autores < 5) {
            printf("Informe o código do próximo autor: ");
            fflush(stdin);
            scanf("%i", &livro.cod_autores[num_autores]);

            fseek(fp_autor, 0, SEEK_SET); // volta para o início do arquivo autor
            autorEncontrado = 0; // reinicia a variável de controle

            while (fread(&autor, sizeof(autor), 1, fp_autor) == 1) { // percorre o arquivo autor
                if (autor.cod_autor == livro.cod_autores[num_autores]) {
                    printf("O autor selecionado e: %s\n", autor.nome_autor);
                    autorEncontrado = 1; // Marca o autor como encontrado
                    break; // Sai do loop assim que o autor for encontrado
                }
            }

            if (!autorEncontrado) {
                printf("Erro ao selecionar o autor.\n");
                break; // Sai do loop de adição de autores
            }

            num_autores++;

            printf("Deseja adicionar mais autores para o livro? (S/N): ");
            fflush(stdin); scanf(" %c", &op);
        }

        if (autorEncontrado) {
            fwrite(&livro, sizeof(livro), 1, fplivro);
            fclose(fplivro);

            printf("\nLivro cadastrado com sucesso.\n");
        }
    } else {
        printf("Erro ao selecionar o autor.\n");
    }

    fclose(fp_autor);
}
	
void listarAutor (){
	  FILE *fp_autor;
  struct reg_autor autor;	
	
  //Abrir o Arquivo
  if ((fp_autor = fopen(AUTOR,"rb"))==NULL){
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }
  
  //Ler registo	por registro e mostrar na tela
  printf("\n##### Relatorio de Todos os AUTORES ######");
  printf("\nCodigo    Autor");
  while (fread(&autor,sizeof(autor),1,fp_autor)==1){
    printf("\n%-10i %s", autor.cod_autor, autor.nome_autor);
  }  
  
  //Fechar o Arquivo
  fclose(fp_autor);	
}

void listarLivros() {
    FILE* fplivro;
    FILE* fp_autor;
    struct reg_livro livro;
    struct reg_autor autor;
    int i;

    // Abrir o arquivo de livros
    if ((fplivro = fopen("livros.dat", "rb")) == NULL) {
        printf("\nErro na abertura do arquivo de livros!");
        return;
    }

    // Abrir o arquivo de autores
    if ((fp_autor = fopen("autor.dat", "rb")) == NULL) {
        printf("\nErro na abertura do arquivo de autores!");
        fclose(fplivro); // Fechar o arquivo de livros antes de retornar
        return;
    }

    // Ler registro por registro e mostrar na tela
    printf("\n##### Relatorio de Todos os Livros #####");
    printf("\nCodigo      Titulo                         Preco");

    while (fread(&livro, sizeof(livro), 1, fplivro) == 1) {
        printf("\n%-6i %-40s %5.2f", livro.codigo, livro.titulo, livro.preco);
        printf("\nAutores do Livro\n");
        printf("----------------\n");

        for (i = 0; i < TAM_AUTOR; i++) {
            // Verifica se o código de autor é válido
            if (livro.cod_autores[i] != 0) {
                // Procurar o registro do autor
                int autorEncontrado = 0;
                rewind(fp_autor); // Voltar ao início do arquivo de autores

                while (fread(&autor, sizeof(autor), 1, fp_autor) == 1) {
                    if (livro.cod_autores[i] == autor.cod_autor) {
                        printf("Autor %d: %s\n", i + 1, autor.nome_autor);
                        autorEncontrado = 1;
                        break;
                    }
                }

                if (!autorEncontrado) {
                    printf("\n");
                }
            }
        }
    }

    // Fechar os arquivos
    fclose(fp_autor);
    fclose(fplivro);
}
void listarLivrosAutor() {
  FILE *fplivro;
  FILE *fp_autor;
  struct reg_livro livro;
  struct reg_autor autor;
  int cod, i, achou = 0;

  // Solicitar o código do Autor a ser consultado
  printf("Digite o Codigo do Autor: ");
  scanf("%i", &cod);
  
  // Abrir o Arquivos
  fplivro = fopen(LIVROS, "rb");
  if (fplivro == NULL) { // Verificar se o arquivo livro foi aberto 
    printf("\nErro na abertura do arquivo de livros!");
    return;
  }
  
  fp_autor = fopen(AUTOR, "rb");
  if (fp_autor == NULL) { // Verificar se o arquivo de autores foi aberto 
    printf("\nErro na abertura do arquivo de autores!");
    fclose(fplivro); // Fechar o arquivo de livros antes de retornar
    return;
  }
  
  // Localizar e mostrar o Livro
  while (fread(&livro, sizeof(livro), 1, fplivro) == 1) {
    for (i = 0; i < TAM_AUTOR; i++) {
      // Ler o registro do autor correspondente ao código do livro
      fseek(fp_autor, livro.cod_autores[i] * sizeof(autor), SEEK_SET);
      fread(&autor, sizeof(autor), 1, fp_autor);
      
      // Verificar se o código de autor é válido
      if (livro.cod_autores[i] == cod) {
        printf("Livro encontrado para o autor de autor %d:\n", cod);
        // Exibir informações do livro
        printf("Título: %s\n", livro.titulo);
        printf("Valor: %f", livro.preco);
        
        // ... (outras informações do livro)
        printf("\n");
        
      }
    }
  }

  if (!achou) {
    printf("\n");
  }

  // Fechar os arquivos
  fclose(fplivro);
  fclose(fp_autor);
}

void consultarLivroCodigo(){
  FILE *fplivro;
  FILE *fp_autor;
  struct reg_livro livro;
  struct reg_autor autor;
  int cod, achou=0;
  int i;
  
  //Solicitar o codigo do Livro a ser Consultado
  printf("\nDigite o Codigo do Livro: ");
  fflush(stdin); scanf("%i",&cod);
  
  //Abrir o Arquivo
  if ((fplivro = fopen(LIVROS,"rb"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&livro,sizeof(livro),1,fplivro)==1)){
  	//printf("\nProcessando Livro de Codigo: %i\n",livro.codigo);
  	if (livro.codigo==cod){
  		printf("\nCodigo: %i\nTitulo: %s\nPreco: %5.2f\n",livro.codigo,livro.titulo,livro.preco);
  		achou=1;
	  }
  }
  fp_autor = fopen(AUTOR, "rb");
	while (fread(&autor, sizeof(autor), 1, fp_autor)) {
    for (i = 0; i < TAM_AUTOR; i++) {
        if (autor.cod_autor == livro.cod_autores[i]) {
            printf("Autor: %s\n", autor.nome_autor);
            break; // Encontrou um autor correspondente, não precisa continuar procurando
        }
    }
}

  if (achou==0){
  	printf("\nNenhum Livro com este Codigo!");
  }
  
  //Fechar o arquivo	
  fclose(fplivro);	
  fclose(fp_autor);
}//Fim consultarLivroCodigo()

void consultarLivroTitulo(){
  FILE *fplivro;
  struct reg_livro livro;
  char tit[30];
  int achou=0;
  
  //Solicitar o Titulo do Livro a ser Consultado
  printf("\nDigite o Titulo do Livro: ");
  fflush(stdin); gets(tit);
  
  //Abrir o Arquivo
  if ((fplivro = fopen(LIVROS,"rb"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&livro,sizeof(livro),1,fplivro)==1)){
  	if (strcmp(livro.titulo,tit)==0){
  		printf("\nCodigo: %i\nTitulo: %s\nPreco: %5.2f\n",livro.codigo,livro.titulo,livro.preco);
  		achou=1;
	  }
  }
  
  if (achou==0){
  	printf("\nNenhum Livro com este Codigo!");
  }
  
  //Fechar o arquivo	
  fclose(fplivro);	
}//Fim consultarLivroTitulo()

void alterarLivro(){
  FILE *fplivro;
  struct reg_livro livro;
  int cod, achou=0;
  char opc;
  
  //Localizar o Livro a ser alterado
  printf("\nDigite o Codigo do Livro a ser alterado: ");
  fflush(stdin); scanf("%i",&cod);
  
  //Abrir o Arquivo
  if ((fplivro = fopen(LIVROS,"rb+"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&livro,sizeof(livro),1,fplivro)==1)){
  	if (livro.codigo==cod){
  		printf("\nCodigo: %i\nTitulo: %s\nPreco: %5.2f\n",livro.codigo,livro.titulo,livro.preco);
  		achou=1;
	  }
  }

  if (achou==0){
  	printf("\nNenhum Livro com este Codigo foi Localizado!");
  	fclose(fplivro);
  	return;  //Volta para o menu principal
  }
  
  //Solicitar os novos dados
  printf("\nDeseja Alterar o Titulo?(S/N) ");
  fflush(stdin); scanf("%c",&opc);
  if ((opc=='S') || (opc=='s')){
  	printf("\nDigite o novo Titulo: ");
  	fflush(stdin); gets(livro.titulo);
  }
  printf("\nDeseja Alterar o Preco?(S/N) ");
  fflush(stdin); scanf("%c",&opc);
  if ((opc=='S') || (opc=='s')){
  	printf("\nDigite o novo Preco: ");
  	fflush(stdin); scanf("%f",&livro.preco);
  }  
  
  printf("\nGravar Alteracao?(S/N) ");
  fflush(stdin); scanf("%c",&opc);
  if ((opc!='S') && (opc!='s')){
    printf("\nOperacao Cancelada!");
    fclose(fplivro);
    return; //Retorna para o menu principal
  }
  
  //salvar o registro alterado	
  fseek(fplivro,-sizeof(livro),1);  //Voltar o ponteiro no arquivo para o registro anterior
  
  fwrite(&livro,sizeof(livro),1,fplivro); //Gravou a alteração
  
  //Fechar o arquivo	
  fclose(fplivro);	
  	
  printf("\nAlteracao Gravada com Sucesso.");	    	
}//Fim alterarLivro()

void excluirLivro(){
  FILE *fplivro, *fplivrosnew;
  struct reg_livro livro;
  int cod, achou=0;
  char opc;
  
  //Localizar o Livro a ser excluído
  printf("\nDigite o Codigo do Livro a ser excluido: ");
  fflush(stdin); scanf("%i",&cod);
  
  //Abrir o Arquivo
  if ((fplivro = fopen(LIVROS,"rb"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&livro,sizeof(livro),1,fplivro)==1)){
  	if (livro.codigo==cod){
  		printf("\nCodigo: %i\nTitulo: %s\nPreco: %5.2f\n",livro.codigo,livro.titulo,livro.preco);
  		achou=1;
	  }
  }
  
  fclose(fplivro);//Fecha o arquivo

  if (achou==0){
  	printf("\nNenhum Livro com este Codigo foi Localizado!");
  	return;  //Volta para o menu principal
  }
    
  //Pedir Confirmação
  printf("\nConfirma Exclusao?(S/N) ");
  fflush(stdin); scanf("%c",&opc);
  if ((opc!='S') && (opc!='s')){
    printf("\nOperacao Cancelada!");
    return; //Retorna para o menu principal
  }  
  
  //Excluir o Livro 
  //copiar todos registros, exceto aquele a ser excluído, para um arquivo temporário(livrosnew.dat)
  fplivro = fopen(LIVROS,"rb"); //Arquivo original
  fplivrosnew = fopen("livrosnew.dat","wb"); //Novo Arquivo
  
  while (fread(&livro,sizeof(livro),1,fplivro)==1){
  	if (livro.codigo!=cod){
  		fwrite(&livro,sizeof(livro),1,fplivrosnew);
	}
  }
  fclose(fplivro);
  fclose(fplivrosnew);
  
  //Excluir o arquivo antigo (livros.dat)	
  system("del livros.dat"); 
  
  //Renomear o novo arquivo (livrosnew.dat) para o nome livros.dat
  system("ren livrosnew.dat livros.dat");
  
  printf("\nLivro excluido com sucesso.");
  
}//Fim excluirLivro()

void cadastrarCliente(){
 struct reg_cliente cliente;
 FILE *fpcliente; //Variável para manipular o Arquivo de Clientes
 char op;
 
 //Entrada dos dados do Livro
 printf("\nCadastro de Cliente: ");
 printf("\n Codigo: ");
 fflush(stdin);	scanf("%i",&cliente.codigo);
 printf(" Nome: ");
 fflush(stdin);	gets(cliente.nome);
 printf(" Fone: ");
 fflush(stdin);	gets(cliente.fone);
 printf(" Email: ");
 fflush(stdin);	gets(cliente.email);


 printf("\nG r a v a r ?(S/N) ");
 fflush(stdin); scanf("%c",&op);
 if ((op=='S') || (op=='s')){
 	//Abrir o arquivo
 	fpcliente = fopen(CLIENTES,"ab"); //Modo "ab" para acrescentar um novo Cliente no arquivo binário 
 	fwrite(&cliente,sizeof(cliente),1,fpcliente); //Gravar no arquivo
 	printf("\nCliente Cadastrado com sucesso.");
 	fclose(fpcliente); //Fecha o arquivo	
 }else{
 	printf("\nCadastro Cancelado!");
 }	
}//Fim cadastrarCliente()

void listarClientes(){
  FILE *fpcliente;
  struct reg_cliente cliente;	
	
  //Abrir o Arquivo
  if ((fpcliente = fopen(CLIENTES,"rb"))==NULL){
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }
  
  //Ler registo	por registro e mostrar na tela
  printf("\n##### Relatorio de Todos os Clientes ######");
  printf("\nCodigo Nome                           Fone            Email               ");
  while (fread(&cliente,sizeof(cliente),1,fpcliente)==1){
    printf("\n%-6i %-30s %-15s %-20s",cliente.codigo,cliente.nome,cliente.fone,cliente.email);
  }  
  
  //Fechar o Arquivo
  fclose(fpcliente);	
}//Fim listarClientes()

struct reg_cliente localizarCliente(int codcli){
  FILE *fpcliente;
  struct reg_cliente cliente;
  int achou=0;
  
  
  //Abrir o Arquivo
  if ((fpcliente = fopen(CLIENTES,"rb"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&cliente,sizeof(cliente),1,fpcliente)==1)){
  	if (cliente.codigo==codcli){
  		achou=1;
	  }
  }

  //Fechar o arquivo	
  fclose(fpcliente);		

  if (achou==0){
  	cliente.codigo=-1;  //Sinaliza que não achou o Cliente
  }
  	
  return cliente;	
}//Fim localizarCliente()

struct reg_livro localizarLivro(int codliv){
  FILE *fplivro;
  struct reg_livro livro;
  int achou=0;
  
  
  //Abrir o Arquivo
  if ((fplivro = fopen(LIVROS,"rb"))==NULL){ //Abre para leitura - Posiciona no primeiro registro
  	printf("\nErro na abertura do arquivo!!");
  	return;
  }  
  
  //Localizar e mostrar o Livro
  while ((achou==0) && (fread(&livro,sizeof(livro),1,fplivro)==1)){
  	if (livro.codigo==codliv){
  		achou=1;
	  }
  }

  //Fechar o arquivo	
  fclose(fplivro);		

  if (achou==0){
  	livro.codigo=-1;  //Sinaliza que não achou o livro
  }
  	
  return livro;	

} //Fim localizarLivro()

void efetuarVenda() {
  FILE *fpvendas;
  struct reg_cliente cliente;
  struct reg_livro livro;
  struct reg_venda venda;
  int codcli, codliv;
  int codvenda = 0;
  char opc;
  
  printf("\nEFETUAR VENDA\n");
  
	fpvendas = fopen("vendas.dat", "rb");
	
  //Solicitar o código do Cliente
	printf("\nDigite o codigo do Cliente: ");
	fflush(stdin); scanf("%i",&codcli);
  
  //Localizar e mostrar o nome do cliente
  cliente = localizarCliente(codcli);
  
  if (cliente.codigo==-1){
  	printf("\nCliente NAO Localizado!");
  	return; //retorna para o menu principal
  }
  printf("\nCliente: %i - %s",cliente.codigo,cliente.nome);
  printf("\nConfirma Cliente?(S/N) ");
  fflush(stdin); scanf("%c",&opc);
  if ((opc=='N')||(opc=='n')){
  	printf("\nCliente NAO Confirmado!");
  	return; //Retorna ao programa principal
  }
  
  fpvendas = fopen("vendas.dat", "rb");
while (fread(&venda, sizeof(venda), 1, fpvendas) == 1) {
  if (venda.vendacodcli == codcli) {
    if (venda.vendacodigo >= codvenda) {
      codvenda = venda.vendacodigo;
    }
  }
}
fclose(fpvendas);

// Incrementar o código de venda
if (codvenda != 0) {
  codvenda++;
} else {
  codvenda = 1; // Definir o código de venda como 1 se for o primeiro pedido do cliente
}

// Exibir o código de venda atual
printf("\nCodigo da venda: %i", codvenda);

  // Solicitar os demais dados do pedido
  // Solicitar o código do Livro a ser vendido
  printf("\nDigite o codigo do Livro a ser vendido: ");
  fflush(stdin);
  scanf("%i", &codliv);
  
  //Localizar e mostrar os dados do Livro
  livro = localizarLivro(codliv);
  
  if (livro.codigo == -1) {
    printf("\nLivro NAO Localizado!");
    return; // retorna para o menu principal
  }
  
  printf("\nLivro: %i - %s", livro.codigo, livro.titulo);
  printf("\nConfirma Titulo?(S/N) ");
  fflush(stdin);
  scanf("%c", &opc);
  if ((opc == 'N') || (opc == 'n')) {
    printf("\nLivro NAO Confirmado!");
    return; // Retorna ao programa principal
  }  
  
  // Solicitar a quantidade do livro a ser vendido
  printf("\nDigite a qtde vendida: ");
  fflush(stdin);
  scanf("%i", &venda.vendaqtde);
  
  // Solicitar o desconto
  printf("\nDigite o desconto: ");
  fflush(stdin);
  scanf("%f", &venda.vendadesc);
  
  // Preencher a struct venda com as demais informações
  venda.vendacodigo = codvenda;
  venda.vendacodcli = codcli;
  venda.vendacodliv = codliv;
  venda.vendasitua = 0;
  
  // Gravar  
  fpvendas = fopen("vendas.dat", "ab");
  fwrite(&venda, sizeof(venda), 1, fpvendas);
  fclose(fpvendas);
  
  printf("\nVenda Gravada com Sucesso.");
}

void relatorioVendas(){
  FILE *fpvendas;
  struct reg_venda venda;
  struct reg_livro livro;
  struct reg_cliente cliente;
  char situa_cliente[20];
  
  //Abrir Arquivo de Vendas
  fpvendas = fopen(VENDAS,"rb");
  
  //Ler registro por registro e imprimir na tela
  while (fread(&venda,sizeof(venda),1,fpvendas)==1){
  	cliente = localizarCliente(venda.vendacodcli);
  	livro = localizarLivro(venda.vendacodliv);
  	printf("\nCodigo Venda: %i",venda.vendacodigo);
  	printf("\nCliente: %i - %s",venda.vendacodcli,cliente.nome);
  	printf("\nLivro: %i - %s",venda.vendacodliv,livro.titulo);
  	printf("\nQtde: %i",venda.vendaqtde);
  	printf("\nPreço: %5.2f - Desconto: %5.2f - Vlr. Pago: %5.2f", livro.preco,venda.vendadesc,(livro.preco*venda.vendaqtde)-venda.vendadesc);
  	 if(venda.vendasitua == 0){
	  strcpy(situa_cliente,"Em aberto");
  }
  if(venda.vendasitua == 1){
	  strcpy(situa_cliente,"Fechado");
  }
	printf("\nPedido: %s",situa_cliente);
	printf("\n------------------------------------------");
}
  //Fechar o arquivo
  fclose(fpvendas);	
}//Fim relatorioVendas()

void relatorioVendasV2(){
  FILE *fpvendas;
  struct reg_venda venda;
  struct reg_livro livro;
  struct reg_cliente cliente;
  char situa_cliente[20];
  
  //Abrir Arquivo de Vendas
  fpvendas = fopen(VENDAS,"rb");
  if(venda.vendasitua == 0){
      strcpy(situa_cliente,"Em aberto");
    }
    if(venda.vendasitua == 1){
      strcpy(situa_cliente,"Fechado");
    }
  printf("\n   ### Livraria TechInfo ###");
  printf("\n Relatorio de Vendas ");
  printf("\nVenda Cliente                   Livro                          Qtde Preco  Desconto Vlr. Pago   Situacao");
  
  //Ler registro por registro e imprimir na tela
  while (fread(&venda,sizeof(venda),1,fpvendas)==1){
  	cliente = localizarCliente(venda.vendacodcli);
  	livro = localizarLivro(venda.vendacodliv);
  	
  	if(venda.vendasitua == 0){
	  strcpy(situa_cliente,"Em aberto");
  }
  if(venda.vendasitua == 1){
	  strcpy(situa_cliente,"Fechado");
  }
  	
  	printf("\n%-5i %-4i %-20s %-4i %-25s %-4i %-5.2f %-8.2f %-5.2f ",
	  venda.vendacodigo,venda.vendacodcli,cliente.nome,venda.vendacodliv,livro.titulo,venda.vendaqtde,livro.preco,venda.vendadesc,(livro.preco*venda.vendaqtde)-venda.vendadesc);
  	printf("      %s",situa_cliente);
  	
  	//printf("\n------------------------------------------");
  }
  
  //Fechar o arquivo
  fclose(fpvendas);	
}//Fim relatorioVendasV2()

void fecharpedido() {
	FILE *fpvendas;
	FILE *fpcliente;
	FILE *fplivro;
	struct reg_livro livro;
	struct reg_cliente cliente;
	struct reg_venda venda;
	int cod_pedido, cod_cliente,codliv;
	int encontrado=0;
	float totalpag = 0.0;
	char opc;
	
	if(fpvendas = fopen(VENDAS,"rb")==NULL){ // abriu o arquivo de vendas
		printf("erro ao abrir o arquivo de vendas/n");
		return;
	}
	printf("informe o codigo do cliente a ser fechado\n");
	scanf("%i", &cod_cliente);
	fpvendas=fopen(VENDAS,"rb"); // abre o arquivo de vendas para buscar o codigo do pedido
	while(fread(&venda, sizeof(struct reg_venda),1, fpvendas) == 1){
		if(cod_cliente == venda.vendacodcli){
			cod_pedido = venda.vendacodigo;
			encontrado = 1;
			printf("cliente encontrado");
		}
		}
		if (encontrado == 0) {
        printf("O codigo do cliente nao foi encontrado.\n");
        return;
    }
	
    fclose(fpvendas); // fecha o arq VENDAS
    
    
    //printf("\n Nro venda: %i",cod_pedido);
	printf("\n Cod cliente - Nome Cliente");
	
	fpcliente=fopen(CLIENTES,"rb");// abre o arquivo de clientes
	while(encontrado==1 && fread(&cliente,sizeof(cliente),1,fpcliente)==1){
		if(cod_cliente == cliente.codigo){	// verifica se o cod cliente é o mesmo cod do cliente no pedido
			printf("\n %-13i %s", cliente.codigo, cliente.nome);
		}
		
	}
	fclose(fpcliente); //fecha o arquivo de clientes
	
	printf("\n COD TITULO               QTDE.                PRECO    DESCONTO     VALOR PAGO");
	fpvendas=fopen(VENDAS,"rb"); // abre o arquibo de venda
	while( fread(&venda,sizeof(venda),1,fpvendas)==1){ //percorre o arquivo de vendas
		if(venda.vendacodcli == cod_cliente && venda.vendasitua == 0){
		// Se o cod no arquivo Vendas for igual ao cod_pedido fornecido 
		//ou o cod cliente vindo do arquivo for igual ao cod_cliente
			fplivro=fopen(LIVROS,"rb"); //abre o arq de livros
					cliente = localizarCliente(venda.vendacodcli); // localiza o cliente
  					livro = localizarLivro(venda.vendacodliv);
		printf("\n%-4i %-20s %-20i %-10.2f %-10.2f %-7.2f",
	    venda.vendacodliv,livro.titulo,venda.vendaqtde,livro.preco,venda.vendadesc,(livro.preco*venda.vendaqtde)-venda.vendadesc);
  		  	  	  	
					  totalpag += livro.preco*venda.vendaqtde-venda.vendadesc;		  
		}
					
	}
	printf("\n------------------------------------------");
	printf("\n %.2f", totalpag);
			
		printf("\nConfirma pagamento do Cliente?(S/N) \n"); // Inicio confirmaçao do pagamento
    	
		fflush(stdin); scanf("%c",&opc);
	  	
if ((opc == 'S') || (opc == 's')) {
    fseek(fpvendas, 0, SEEK_SET);  // Posiciona o indicador de posição do arquivo no início da struct registro de venas
    fpvendas = fopen(VENDAS,"rb+"); // abre o resgistro para leitura e alteração
    
    while (fread(&venda, sizeof(venda), 1, fpvendas) == 1) {
    	if(venda.vendacodcli == cod_cliente && venda.vendasitua == 0){
        venda.vendasitua = 1;
        
        fseek(fpvendas, -sizeof(venda), SEEK_CUR);  // Volta para a posição atual do registro
        fwrite(&venda, sizeof(venda), 1, fpvendas);  // Grava a alteração
        
        fseek(fpvendas, 0, SEEK_CUR);  // Volta para a posição atual do indicador
    }
	}
    
    fclose(fpvendas);
    printf("Pedido fechado com sucesso !!!");
} else {
    printf("\n Operacao cancelada !!");
	return;
}
fclose(fpvendas);
}
void main() {
  int op;
  
  do {
  	
  	printf("\n\n########## Sistema Controle Livraria ##############");
  	printf("\n###          Livraria TechInfo                  ###");
  	printf("\n### 1) Cadastrar novo Autor                     ###");
  	printf("\n### 11) Cadastrar novo Livro                     ###");
  	printf("\n### 111) Cadastrar novo Cliente                   ###");
  	printf("\n### 2) Listar Todos os Autor                      ###");
  	printf("\n### 22) Listar Todos os Livros                    ###");
  	printf("\n### 222) Listar Todos os Livros por COD_AUTOR     ###");
  	printf("\n### 2222) Listar Todos os Clientes                 ###");
  	printf("\n### 3) Consultar Livro pelo Codigo                ###");
  	printf("\n### 4) Consultar Livro pelo Titulo                ###");
  	printf("\n### 5) Consulta Livro por palavra chave no Título ###");
  	printf("\n### 6) Alterar dados de um Livro                  ###");
  	printf("\n### 7) Excluir um Livro                           ###");
  	printf("\n### 8) Efetuar Venda                              ###");
  	printf("\n### 9) Relatorio de Vendas                        ###");
  	printf("\n### 91) Relatorio de Vendas versao horizontal     ###");
  	printf("\n### 10) Fechar Pedido                             ###");
  	printf("\n###                                               ###");
  	printf("\n### 0) Sair                                       ###");
  	printf("\n\nOpcao-> ");
  	fflush(stdin); scanf("%i",&op);
  	
  	switch(op){
  		 case 1:
		cadastrarAutor();
		break;
  	  case 11:
		cadastrarLivro();
		break;
	  case 111: //Cadastrar novo Cliente
	  	cadastrarCliente();
		break;
		case 2:
	  	listarAutor();
	  	break;
		  case 22:
	  	listarLivros();
	  	break;	
	  case 222:
	  	listarLivrosAutor();
	  	break;
	  case 2222:
	  	listarClientes();
	  	break;
	  case 3:
	  	consultarLivroCodigo();
	  	break;
	  case 4:
	  	consultarLivroTitulo();
	  	break;
	  case 6:
	  	alterarLivro();
	  	break;
	  case 7:
	    excluirLivro();
		break;	
	  case 8: //Efetuar venda
	    efetuarVenda();
	    break;
	  case 9: //Relatorio de Vendas
	    relatorioVendas();
	    break;
	  case 91: //Relatorio de vendas horizontal
	    relatorioVendasV2();
	    break;	
		case 10: //Fechar o Pedido
	    fecharpedido();
	    break;	    
	}
	
  	 	
  }while (op!=0);
 
}//Fim main
