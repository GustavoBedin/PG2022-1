#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

// GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Prot?tipo da fun??o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot?tipos das fun??es
int setupGeometry();

// Dimens?es da janela (pode ser alterado em tempo de execu??o)
const GLuint WIDTH = 800, HEIGHT = 600;

enum Movement { Left, Right, Top, Down, Stop };
static Movement moves = Stop;
// Fun??o MAIN
int main()
{
	// Inicializa??o da GLFW
	glfwInit();

	// Cria??o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Gustavo", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun??o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa??es de vers?o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;




	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri?ngulo
	GLuint VAO = setupGeometry();
	

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a vari?veis do tipo uniform em GLSL para armazenar esse tipo de info
	// que n?o est? nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);
	
	//Criando a matriz de proje??o usando a GLM
	glm::mat4 projection = glm::mat4(1);// Matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	float x = 400;
	float y = 300;
	float angle = 0.0;

	// Loop da aplica??o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens?es da viewport com as mesmas dimens?es da janela da aplica??o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		 //Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);


		glm::mat4 model = glm::mat4(1); // Matriz identidade

		switch (moves) {
			case Left:
				x -= 2.0;
				angle = 90;
				break;
			case Right:
				x += 2.0;
				angle = 270;
				break;
			case Top:
				y += 2.0;
				angle = 0;
				break;
			case Down:
				y -= 2.0;
				angle = 180;
				break;
			default:
				break;
		}

		model = glm::translate(model, glm::vec3(x, y, 0));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 0, 1));
		shader.setMat4("model", glm::value_ptr(model));

		glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glViewport(0, 0, width, height);



		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu??o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun??o de callback de teclado - s? pode ter uma inst?ncia (deve ser est?tica se
// estiver dentro de uma classe) - ? chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		moves = Top;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		moves = Down;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		moves = Left;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		moves = Right;
	else 
		moves = Stop;
}


// Esta fun??o est? bastante harcoded - objetivo ? criar os buffers que armazenam a 
// geometria de um tri?ngulo
// Apenas atributo coordenada nos v?rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun??o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri?ngulo e as armazenamos de forma
	// sequencial, j? visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v?rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO ?nico ou em VBOs separados
	GLfloat vertices[] = {
		0.0, 0.0, 0.0,
		 100.0, 0.0, 0.0,
		 50.0, 50.0, 0.0
		 //outro triangulo vai aqui

	};

	GLuint VBO, VAO;

	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza??o no shader * (a localiza??o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est? normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

