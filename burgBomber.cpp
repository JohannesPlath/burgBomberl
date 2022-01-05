
// Include Standardheader, steht bei jedem C/C++-Programm am Anfang
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION  // hilfsframework zum texturen laden
#include "libs/stb_image.h" 
int32_t textureWidth = 0;
int32_t tetureHight = 0;
int32_t bitsPerPixel = 0;
//stbi_set_flip_vertically_on_load(true);




// Include GLEW, GLEW ist ein notwendiges Übel. Der Hintergrund ist, dass OpenGL von Microsoft
// zwar unterstützt wird, aber nur in einer Uralt-Version. Deshalb beinhaltet die Header-Datei,
// die vom Betriebssystem zur Verfügung gestellt wird, nur Deklarationen zu den uralten Funktionen,
// obwohl der OpenGL-Treiber, und damit die OpenGL-dll die neuesten Funktionen implementiert.
// Die neueren Funktionen werden deshalb über diese Header-Datei separat zur Verfügung gestellt.
#include <GL/glew.h>

GLuint TextureRasen;
GLuint TextureMauer;
GLuint TextureMauer2;
GLuint TextureMauer3;
GLuint TextureBergMitSee;
GLuint TextureAuge;
GLuint TextureComicBaum;



// Include GLFW, OpenGL definiert betriebssystemunabhängig die graphische Ausgabe. Interaktive 
// Programme beötigen aber natürlich auch Funktionen für die Eingabe (z. B. Tastatureingaben)
// Dies geht bei jedem OS (z. B. Windows vs. MacOS/Unix) etwas anders. Um nun generell plattformunabhängig
// zu sein, verwenden wir GLFW, was die gleichen Eingabe-Funktionen auf die Implementierung unterschiedlicher
// OS abbildet. (Dazu gibt es Alternativen, glut wird z. B. auch häufig verwendet.)
#include <GLFW/glfw3.h>

// Include GLM, GLM definiert für OpenGL-Anwendungen Funktionen der linearen Algebra wie
// Transformationsmatrizen. Mann könnte GLM auch durch etaws anderes ersetzen oder aber in einem
// anderen Kontext verwenden.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// In C- und C++-Programmen ist die Reihenfolge der include-Direktiven wichtig.
// Dateien, die mit Spitzklammern includiert werden, werden in den System-Verzeichnissen
// gesucht, die mit doppelten Hochkommata im lokalen Projektverzeichnis
// (wo genau ist in den Projekteinstellungen zu finden und ggf. zu ändern.) 

// Diese Datei benötigen wir, um die Shader-Programme komfortabel in die Hardware zu laden.
// (Mit der rechten Mouse-taste können Sie in VS diese Datei öffnen, um nachzuschauen, was dort deklariert wird.)
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"
#include <iostream>

// 3DObjekt als Klasse/Object
#include "Obj3D.hpp"
// Obj3D cubeObj("cube.obj") ;
Obj3D* cubeObj = NULL;
Obj3D* CubeFullTexure360 = NULL;
Obj3D* mauerObj = NULL;
Obj3D* zylinderObj = NULL;
Obj3D* turmObject = NULL;
Obj3D* kanneObject = NULL;


// globala variablen
float flugobjektX;
float flugobjektY;
float flugobjektZ;


float winkel = 0;
void addWinkel(float w) {
	winkel = winkel + w;
}
// globala variablen
float kanneLinksYrotate = 90;
float kanneRechtsYrotate = 90;

void reduceWinkel(float w) {
	winkel = winkel - w;
}
float xWert = 1;
float yWert = 1;
float zWert = 1;
int varGL_DIVERS = GL_LESS;
void setGL_Greater() {
	glClearDepthf(0.0f);
	varGL_DIVERS = GL_GREATER;
}
void setGL_Less() {
	glClearDepthf(1.0f);
	varGL_DIVERS = GL_LESS;
}

float trim = 0.1;
void trim_more() {
	trim = trim + 0.5;
}
void trim_less() {
	trim = trim - 0.5;
}
float rotate2 = 0.1;
void rotate_More() {
	rotate2 = rotate2 + 0.5;

}
void rotate_Less() {
	rotate2 = rotate2 - 0.5;

}
float spin = 0.1;
void spin_More() {
	spin = spin + 0.5;

}
void spin_Less() {
	spin = spin - 0.5;

}
float rXwert = 0.1;
void rXWert_More() {
	rXwert = rXwert + 0.5;

}
void rXWert_less() {
	rXwert = rXwert - 0.5;

}
float rYwert = 0.1;
void rYWert_More() {
	rYwert = rYwert + 0.5;

}
void rYWert_less() {
	rYwert = rYwert - 0.5;

}
float rZwert = 0.1;
void rZWert_More() {
	rZwert = rZwert + 0.5;
}
void rZWert_less() {
	rZwert = rZwert - 0.5;
}
float roboWinkel = 1;
float rotate3 = 1;




// Callback-Mechanismen gibt es in unterschiedlicher Form in allen möglichen Programmiersprachen,
// sehr häufig in interaktiven graphischen Anwendungen. In der Programmiersprache C werden dazu 
// Funktionspointer verwendet. Man übergibt einer aufgerufenen Funktion einer Bibliothek einen
// Zeiger auf eine Funktion, die zurückgerufen werden kann. Die Signatur der Funktion muss dabei
// passen. Dieser Mechanismus existiert auch in C++ und wird hier verwendet, um eine einfache
// Fehlerbehandlung durchzuführen. Diese Funktion gibt Fehler aus, die beim Aufruf von OpenGL-Befehlen
// auftreten.
void error_callback(int error, const char* description)
{
	// Mit fputs gibt man hier den String auf den Standarderror-Kanal aus.
	// In der C-Welt, aus der das hier übernommen ist, sind Strings Felder aus "char"s, die mit 
	// dem Wert null terminiert werden.
	fputs(description, stderr);
}

// Diese Funktion wird ebenfalls über Funktionspointer der GLFW-Bibliothek übergeben.
// (Die Signatur ist hier besonders wichtig. Wir sehen, dass hier drei Parameter definiert
//  werden müssen, die gar nicht verwendet werden.)
// Generell überlassen wir der GLFW-Bibliothek die Behandlung der Input-Ereignisse (Mouse moved,
// button click, Key pressed, etc.).
// Durch die Übergabe dieser Funktion können wir Keyboard-Events 
// abfangen. Mouse-Events z. B. erhalten wir nicht, da wir keinen Callback an GLFW übergeben.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		// Mit rechte Mousetaste -> gehe zu Deklaration finden Sie andere Konstanten für Tasten.
	case GLFW_KEY_ESCAPE:
		// Das Programm wird beendet, wenn BenutzerInnen die Escapetaste betätigen.
		// Wir könnten hier direkt die C-Funktion "exit" aufrufen, eleganter ist aber, GLFW mitzuteilen
		// dass wir das Fenster schliessen wollen (siehe Schleife unten).
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_W:
		zWert = zWert - 1;
		break;
	case GLFW_KEY_Q:
		zWert = zWert + 1;
		break;
	case GLFW_KEY_E:
		kanneLinksYrotate = kanneLinksYrotate - 5;
		break;
	case GLFW_KEY_R:
		kanneLinksYrotate = kanneLinksYrotate + 5;
		break;
	case GLFW_KEY_Y:
		kanneRechtsYrotate = kanneRechtsYrotate - 5;
		break;
	case GLFW_KEY_U:
		kanneRechtsYrotate = kanneRechtsYrotate + 5;
		break;
	case GLFW_KEY_X:
		xWert = xWert - 1;
		break;
	case GLFW_KEY_S:
		xWert = xWert + 1;
		break;
	case GLFW_KEY_D:
		yWert = yWert - 1;
		break;
	case GLFW_KEY_A:
		yWert = yWert + 1;
		break;
	case GLFW_KEY_G:
		setGL_Greater();
		break;
	case GLFW_KEY_H:
		setGL_Less();
		break;
	case GLFW_KEY_KP_1:
		rotate_Less();
		break;
	case GLFW_KEY_KP_4:
		rotate_More();
		break;
	case GLFW_KEY_KP_2:
		trim_less();
		break;
	case GLFW_KEY_KP_3:
		trim_more();
		break;
	case GLFW_KEY_KP_5:
		spin_Less();
		break;
	case GLFW_KEY_KP_6:
		spin_More();
		break;
	case GLFW_KEY_KP_7:
		rXWert_More();
		break;
	case GLFW_KEY_KP_8:
		rYWert_More();
		break;
	case GLFW_KEY_KP_9:
		rZWert_More();
		break;
	case GLFW_KEY_O:
		roboWinkel = roboWinkel + 1;
		break;
	case GLFW_KEY_P:
		roboWinkel = roboWinkel - 1;
		break;
	case GLFW_KEY_K:
		rotate3 = rotate3 - 1;
		break;
	case GLFW_KEY_L:
		rotate3 = rotate3 + 1;
		break;
	default:
		break;
	}
}


// Diese drei Matrizen speichern wir global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen. Ihre Bedeutung habe ich in der Vorlesung Geometrische
// Transformationen erklärt, falls noch nicht geschehen, jetzt anschauen !
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID; // OpenGL unterstützt unterschiedliche Shaderprogramme, zwischen denen man
				  // wechseln kann. Unser Programm wird mit der unsigned-integer-Variable programID
				  // referenziert.

// Ich habe Ihnen hier eine Hilfsfunktion definiert, die wir verwenden, um die Transformationsmatrizen
// zwischen dem OpenGL-Programm auf der CPU und den Shaderprogrammen in den GPUs zu synchronisieren.
// (Muss immer aufgerufen werden, bevor wir Geometriedaten in die Pipeline einspeisen.)
void sendMVP()
{
	// Zunächst können wir die drei Matrizen einfach kombinieren, da unser einfachster Shader
	// wirklich nur eine Transformationsmatrix benötigt, wie in der Vorlesung erklärt.
	// Später werden wir hier auch die Teilmatrizen an den Shader übermitteln müssen.
	// Interessant ist hier, dass man in C++ (wie auch in C#) den "*"-Operator überladen kann, so dass
	// man Klassenobjekte miteinander multiplizieren kann (hier Matrizen bzw. "mat4"), 
	// das ginge in JAVA so natürlich nicht. 
	glm::mat4 MVP = Projection * View * Model;

	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);


	// "glGetUniformLocation" liefert uns eine Referenz auf eine Variable, die im Shaderprogramm
	// definiert ist, in diesem Fall heisst die Variable "MVP".
	// "glUniformMatrix4fv" überträgt Daten, genauer 4x4-Matrizen, aus dem Adressraum unserer CPU
	// (vierter Parameter beim Funktionsaufruf, wir generieren mit "&" hier einen Pointer auf das erste 
	//  Element, und damit auf das gesamte Feld bzw den Speicherbereich) 
	// in den Adressraum der GPUs. Beim ersten Parameter 
	// muss eine Referenz auf eine Variable im Adressraum der GPU angegeben werden.
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
}

//Obj3D cubeObj("cube.obj");

void drawGround(Obj3D* obj3D)
{
	glm::mat4 Save = Model;
	glBindTexture(GL_TEXTURE_2D, TextureRasen);
	Model = glm::translate(Model, glm::vec3(0, -1, 0));
	Model = glm::scale(Model, glm::vec3(5, 0.01, 5));
	sendMVP();
	obj3D->display();

	//steine Im Boden
	float BodenSteineArray1[][2] = { {-12,0},{12,-2}, {-4, 9.5}, {9, 3},{ -8, 9}, {-6, 5.5}, {5, -9}, {9, -11},{ 7, 11}, {-10, 7}, {-3.5,-3.25}, {-2.5,-6.5},{2, 0}, {2.7, 6.3}, {-0 ,-4},  {-8.6, 4.7 }, { 7.4,-7.4 }, { -5.5, 6.5 }, { 0, 3.6 }, { -7.5, -11.5 }, { -4,-2 }, { -8, -5 }, { 5.5, 7 }, { 1.5 , 8.5 } };
	glBindTexture(GL_TEXTURE_2D, TextureMauer);

	for (int x = 0; x < sizeof(BodenSteineArray1) / sizeof(BodenSteineArray1[0]); x += 1) {
		Model = Save;
		Model = glm::scale(Model, glm::vec3(0.2, 0.2, 0.4));
		Model = glm::translate(Model, glm::vec3(BodenSteineArray1[x][0], -5.9, BodenSteineArray1[x][1]));
		sendMVP();
		mauerObj->display();
	};

	Model = Save;
}
void drawZylinder(Obj3D* obj3D)
{ // linke seite 
	glm::mat4 Save = Model;
	glBindTexture(GL_TEXTURE_2D, TextureMauer3);
	Model = glm::translate(Model, glm::vec3(4.0, -0.45, 0));
	Model = glm::scale(Model, glm::vec3(0.5, 1, 1));
	sendMVP();
	obj3D->display();
	Model = Save;
	// rechte Seite	

	Model = glm::translate(Model, glm::vec3(-4.0, -0.45, 0));
	Model = glm::scale(Model, glm::vec3(0.5, 1, 1));
	sendMVP();
	obj3D->display();
	Model = Save;
}

void drawBackRound(Obj3D* obj)
{	//hinten
	glm::mat4 Save = Model;
	glBindTexture(GL_TEXTURE_2D, TextureBergMitSee);
	Model = glm::translate(Model, glm::vec3(0.2, 4.5, 4.8));
	Model = glm::scale(Model, glm::vec3(5.5, 3, 0.01));
	sendMVP();
	obj->display();
	Model = Save;

	//Links
	glBindTexture(GL_TEXTURE_2D, TextureBergMitSee);
	Model = glm::translate(Model, glm::vec3(5.1, 4.5, 0));
	Model = glm::scale(Model, glm::vec3(0.01, 3, 5.5));
	sendMVP();
	obj->display();
	Model = Save;

	//rechts	
	glBindTexture(GL_TEXTURE_2D, TextureBergMitSee);
	Model = glm::translate(Model, glm::vec3(-4.8, 4.5, 0));
	Model = glm::scale(Model, glm::vec3(0.01, 3, 5.5));
	sendMVP();
	obj->display();
	Model = Save;
}


void drawBlock(Obj3D* opj3D, float scX, float scY, float scZ, float trX, float trY, float trZ)
{
	glm::mat4 Save = Model;
	glBindTexture(GL_TEXTURE_2D, TextureMauer);
	Model = glm::translate(Model, glm::vec3(trX, trY, trZ));
	Model = glm::scale(Model, glm::vec3(scX, scY, scZ));
	sendMVP();
	opj3D->display();
	Model = Save;
}

void drawTeeKanne(Obj3D* obj3D, float trX, float trY, float trZ, float rotateY) {
	glm::mat4 Save = Model;
	glBindTexture(GL_TEXTURE_2D, TextureAuge);
	Model = glm::translate(Model, glm::vec3(trX, trY, trZ));
	Model = glm::rotate(Model, rotateY, glm::vec3(0, 1, 0));	 //y-achse
	Model = glm::rotate(Model, -90.0f, glm::vec3(1, 0, 0));   // x-Achse
	Model = glm::scale(Model, glm::vec3(1.0 / 1300.0, 1.0 / 1300.0, 1.0 / 500.0));
	sendMVP();
	obj3D->display();
	Model = Save;
}

void drawSeg(float h)
{
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0, 0, 0.5 * h));
	Model = glm::scale(Model, glm::vec3(0.1, 0.1, h * 0.5));
	sendMVP();
	drawSphere(10, 10);
	Model = Save;
}


void eraseVisibility(float array[][7], int num) {
	for (int x = 0; x < num; x += 1) {
		/*std::cout << "  flugobjektX: " << flugobjektX;
		std::cout << "  flugobjektY: " << flugobjektY;
		std::cout << "  flugobjektZ: " << flugobjektZ;*/
		if (((array[x][3] <= flugobjektX + 0.25) && (array[x][3] >= flugobjektX - 0.25)) &&
			((array[x][4] <= flugobjektY + 0.2) && (array[x][4] >= flugobjektY - 0.2)) &&
			((array[x][5] <= flugobjektZ + 0.5) && (array[x][5] >= flugobjektZ - 0.5))) {
			if (array[x][6]) {
				array[x][6] = 0;
			}
		};
	};
};

void drawTower(float turmArray[][7], int num) {
	for (int x = 0; x < num; x += 1) {
		if (turmArray[x][6] == 1) {
			drawBlock(turmObject, turmArray[x][0], turmArray[x][1], turmArray[x][2], turmArray[x][3], turmArray[x][4], turmArray[x][5]);
		}
	}
}

void drawWall(float  mauerArray[][7], int num) {
	for (int x = 0; x < num; x += 1) {
		if (mauerArray[x][6] == 1) {
			drawBlock(mauerObj, mauerArray[x][0], mauerArray[x][1], mauerArray[x][2], mauerArray[x][3], mauerArray[x][4], mauerArray[x][5]);
		}
	}
}

//create multiple light sources
void enlightenScene()
{
	glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.4f, 0.4f, 0.4f)
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 5.0f, 0.0f),
	glm::vec3(-5.0f, 5.0f, 0.0f),
	glm::vec3(5.0f, 5.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
	};

	// Dir light
	glUniform3f(glGetUniformLocation(programID, "dirLight_position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(programID, "dirLight_ambient"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	glUniform3f(glGetUniformLocation(programID, "dirLight_diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	// Point light 1
	glUniform3f(glGetUniformLocation(programID, "pointLights1_position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(programID, "pointLights1_ambient"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	glUniform3f(glGetUniformLocation(programID, "pointLights1_diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	// Point light 2
	glUniform3f(glGetUniformLocation(programID, "pointLights2_position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(programID, "pointLights2_ambient"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	glUniform3f(glGetUniformLocation(programID, "pointLights2_diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
}


// Einstiegspunkt für C- und C++-Programme (Funktion), Konsolenprogramme könnte hier auch Parameter erwarten
int main(void)
{
	// Initialisierung der GLFW-Bibliothek
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// Öffnen eines Fensters für OpenGL, die letzten beiden Parameter sind hier unwichtig
	// Diese Funktion darf erst aufgerufen werden, nachdem GLFW initialisiert wurde.
	// (Ggf. glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen -> MacOSX)
	GLFWwindow* window = glfwCreateWindow(1280, // Breite
		720,  // Hoehe
		"Castle Bombing", // Ueberschrift
		NULL,  // windowed mode
		NULL); // shared window

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Wir könnten uns mit glfwCreateWindow auch mehrere Fenster aufmachen...
	// Spätestens dann wäre klar, dass wir den OpenGL-Befehlen mitteilen müssen, in
	// welches Fenster sie "malen" sollen. Wir müssen das aber zwingend auch machen,
	// wenn es nur ein Fenster gibt.

	// Bis auf weiteres sollen OpenGL-Befehle in "window" malen.
	// Ein "Graphic Context" (GC) speichert alle Informationen zur Darstellung, z. B.
	// die Linienfarbe, die Hintergrundfarbe. Dieses Konzept hat den Vorteil, dass
	// die Malbefehle selbst weniger Parameter benötigen.
	// Erst danach darf man dann OpenGL-Befehle aufrufen !
	glfwMakeContextCurrent(window);

	// Initialisiere GLEW
	// (GLEW ermöglicht Zugriff auf OpenGL-API > 1.1)
	glewExperimental = true; // Diese Zeile ist leider notwendig.

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Auf Keyboard-Events reagieren (s. o.)
	glfwSetKeyCallback(window, key_callback);

	// Setzen von Dunkelblau als Hintergrundfarbe (erster OpenGL-Befehl in diesem Programm).
	// Beim späteren Löschen gibt man die Farbe dann nicht mehr an, sondern liest sie aus dem GC
	// Der Wertebereich in OpenGL geht nicht von 0 bis 255, sondern von 0 bis 1, hier sind Werte
	// fuer R, G und B angegeben, der vierte Wert alpha bzw. Transparenz ist beliebig, da wir keine
	// Transparenz verwenden. Zu den Farben sei auf die entsprechende Vorlesung verwiesen !
	glClearColor(0.0f, 0.5f, 0.9f, 0.45f);

	// Kreieren von Shadern aus den angegebenen Dateien, kompilieren und linken und in
	// die Grafikkarte übertragen.  
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Diesen Shader aktivieren ! (Man kann zwischen Shadern wechseln.) 
	glUseProgram(programID);


	// erstellern der Teekanne
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("teapot.obj", vertices, uvs, normals);

	// Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	GLuint VertexArrayIDTeapot;
	glGenVertexArrays(1, &VertexArrayIDTeapot);
	glBindVertexArray(VertexArrayIDTeapot);


	// einen Qauder einbinden
	/*GLuint VertexArrayIDQuader;
	glGenVertexArrays(2, &VertexArrayIDQuader);
	glBindVertexArray(VertexArrayIDQuader);*/



	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
	// Buffer zugreifbar für die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0 
	glVertexAttribPointer(0,  // location = 0 
		3,  // Datenformat vec3: 3 floats fuer xyz 
		GL_FLOAT,
		GL_FALSE, // Fixedpoint data normalisieren ?
		0, // Eckpunkte direkt hintereinander gespeichert
		(void*)0); // abweichender Datenanfang ? 

	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 
	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Load the texture
	TextureRasen = loadBMP_custom("rasen3000x2000.bmp");
	TextureMauer = loadBMP_custom("Felsmauer5Groß.bmp");
	TextureMauer2 = loadBMP_custom("Felsmauer6Groß.bmp");
	TextureMauer3 = loadBMP_custom("Felsmauer7.bmp");
	TextureBergMitSee = loadBMP_custom("Berge.bmp");
	TextureAuge = loadBMP_custom("auge.bmp");
	TextureComicBaum = loadBMP_custom("comicBaum.bmp");

	//GLuint TextureMandrill = loadBMP_custom("mandrill.bmp");

	// load the Objects
	kanneObject = new Obj3D("teapot.obj");
	cubeObj = new Obj3D("cube.obj");
	CubeFullTexure360 = new Obj3D("CubeFullTexure360.obj");
	mauerObj = new Obj3D("FelsmauerCube.obj");
	zylinderObj = new Obj3D("zylinder.obj");
	turmObject = new Obj3D("turm.obj");

	// Array Fuer scale "0-2" Mauerposition(translate) "3-5" und  "visible =1 an [x][6]"	// scale laengs: (0.1, 0.2, 0.5)  // scale quer: (0.5, 0.2, 0.1)
	float mauerArray[][7] = {
		{0.1, 0.2, 0.5,   3, -0.8, -2,   1},			// 1-te Schicht laengs - links
		{0.1, 0.2, 0.5,   3, -0.8, -1,   1},
		{0.1, 0.2, 0.5,   3, -0.8,  0,   1},
		{0.1, 0.2, 0.5,   3, -0.8,  1,   1},
		{0.1, 0.2, 0.5,   3, -0.8,  2,   1},
		{0.1, 0.2, 0.5,   3, -0.4, -2,   1},			// 2-te Schicht laengs - links
		{0.1, 0.2, 0.5,   3, -0.4, -1,   1},
		{0.1, 0.2, 0.5,   3, -0.4,  0,   1},
		{0.1, 0.2, 0.5,   3, -0.4,  1,   1},
		{0.1, 0.2, 0.5,   3, -0.4,  2,   1},
		{0.1, 0.2, 0.5,   3, -0.0, -2,   1},			// 3-te Schicht laengs - links
		{0.1, 0.2, 0.5,   3, -0.0, -1,   1},
		{0.1, 0.2, 0.5,   3, -0.0,  0,   1},
		{0.1, 0.2, 0.5,   3, -0.0,  1,   1},
		{0.1, 0.2, 0.5,   3, -0.0,  2,   1},
		{0.5, 0.2, 0.1,	 3.5, -0.8, -2.4,  1},			// 1-3te Schicht quer Links unten rechte Hälfte
		{0.5, 0.2, 0.1,	 3.5, -0.4, -2.4,  1},
		{0.5, 0.2, 0.1,	 3.5, -0.0, -2.4,  1},
		{0.5, 0.2, 0.1,	 4, -0.8, -2.4,  1},			// 1-3te Schicht quer Links unten linke Hälfte
		{0.5, 0.2, 0.1,	 4, -0.4, -2.4,  1},
		{0.5, 0.2, 0.1,	 4, -0.0, -2.4,  1},
		{0.5, 0.2, 0.1,	 3.5, -0.8, 2.4,  1},			// 1-3te Schicht quer Links oben rechte Hälfte
		{0.5, 0.2, 0.1,	 3.5, -0.4, 2.4,  1},
		{0.5, 0.2, 0.1,	 3.5, -0.0, 2.4,  1},
		{0.5, 0.2, 0.1,	 4, -0.8, 2.4,  1},				// 1-3te Schicht quer Links oben linke Hälfte
		{0.5, 0.2, 0.1,	 4, -0.4, 2.4,  1},
		{0.5, 0.2, 0.1,	 4, -0.0, 2.4,  1},
	};
	float mauerArrayRechts[][7] = {
			{0.1, 0.2, 0.5,   -3, -0.8, -2,   1},			// 1-te Schicht laengs - rechts
			{0.1, 0.2, 0.5,   -3, -0.8, -1,   1},
			{0.1, 0.2, 0.5,   -3, -0.8,  0,   1},
			{0.1, 0.2, 0.5,   -3, -0.8,  1,   1},
			{0.1, 0.2, 0.5,   -3, -0.8,  2,   1},
			{0.1, 0.2, 0.5,   -3, -0.4, -2,   1},			// 2-te Schicht laengs - rechts
			{0.1, 0.2, 0.5,   -3, -0.4, -1,   1},
			{0.1, 0.2, 0.5,   -3, -0.4,  0,   1},
			{0.1, 0.2, 0.5,   -3, -0.4,  1,   1},
			{0.1, 0.2, 0.5,   -3, -0.4,  2,   1},
			{0.1, 0.2, 0.5,   -3, -0.0, -2,   1},			// 3-te Schicht laengs - rechts
			{0.1, 0.2, 0.5,   -3, -0.0, -1,   1},
			{0.1, 0.2, 0.5,   -3, -0.0,  0,   1},
			{0.1, 0.2, 0.5,   -3, -0.0,  1,   1},
			{0.1, 0.2, 0.5,   -3, -0.0,  2,   1},
			{0.5, 0.2, 0.1,	 -3.5, -0.8, -2.4,  1},			// 1-3te Schicht quer rechts unten rechte Hälfte
			{0.5, 0.2, 0.1,	 -3.5, -0.4, -2.4,  1},
			{0.5, 0.2, 0.1,	 -3.5, -0.0, -2.4,  1},
			{0.5, 0.2, 0.1,	 -4, -0.8, -2.4,  1},			// 1-3te Schicht quer rechts unten linke Hälfte
			{0.5, 0.2, 0.1,	 -4, -0.4, -2.4,  1},
			{0.5, 0.2, 0.1,	 -4, -0.0, -2.4,  1},
			{0.5, 0.2, 0.1,	 -3.5, -0.8, 2.4,  1},			// 1-3te Schicht quer rechts oben rechte Hälfte
			{0.5, 0.2, 0.1,	 -3.5, -0.4, 2.4,  1},
			{0.5, 0.2, 0.1,	 -3.5, -0.0, 2.4,  1},
			{0.5, 0.2, 0.1,	 -4, -0.8, 2.4,  1},				// 1-3te Schicht quer rechts oben linke Hälfte
			{0.5, 0.2, 0.1,	 -4, -0.4, 2.4,  1},
			{0.5, 0.2, 0.1,	 -4, -0.0, 2.4,  1},
	};
	// Array fuer Tuerme
	float turmArrayLinks[][7] = { {0.2, 0.3, 0.2, 3, -0.25, -2.5, 1},
								  {0.2, 0.3, 0.2, 3, -0.25, 2.5, 1},
								  {0.2, 0.4, 0.2, 4.5, -0.25, -2.5, 1},
								  {0.2, 0.4, 0.2, 4.5, -0.25, 2.5, 1} };
	float turmArrayRechts[][7] = { {0.2, 0.3, 0.2,-3, -0.25, -2.5, 1},
								  {0.2, 0.3, 0.2, -3, -0.25, 2.5, 1},
								  {0.2, 0.4, 0.2, -4.5, -0.25, -2.5, 1},
								  {0.2, 0.4, 0.2, -4.5, -0.25, 2.5, 1} };

	// Alles ist vorbereitet, jetzt kann die Eventloop laufen...
	while (!glfwWindowShouldClose(window))
	{
		// Löschen des Bildschirms (COLOR_BUFFER), man kann auch andere Speicher zusätzlich löschen, 
		// kommt in späteren Übungen noch...
		// Per Konvention sollte man jedes Bild mit dem Löschen des Bildschirms beginnen, muss man aber nicht...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(varGL_DIVERS);

		// Einstellen der Geometrischen Transformationen
		// Wir verwenden dazu die Funktionen aus glm.h
		// Projektionsmatrix mit 45Grad horizontalem Öffnungswinkel, 4:3 Seitenverhältnis, 
		// Frontplane bai 0.1 und Backplane bei 100. (Das sind OpenGL-Einheiten, keine Meter oder der gleichen.)
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		// Viewmatrix, beschreibt wo die Kamera steht, wo sie hinschaut, und wo oben ist. 
		// Man muss angeben, wo oben ist, da es eine Mehrdeutigkeit gäbe, wenn man nur beschreiben
		// würde, wo die Kamera steht und wo sie hinschaut. Denken Sie an ein Flugzeug. Die Position 
		// des/r Piloten/in in der Welt ist klar, es ist dann auch klar, wo er/sie hinschaut. Das Flugzeug 
		// kann sich aber z. B. auf die Seite legen, dann würde der Horizont "kippen". Dieser Aspekt wird
		// mit dem up-Vektor (hier "oben") gesteuert.
		View = glm::lookAt(glm::vec3(0, 4, -10), // die Kamera ist bei (0,0,-5), in Weltkoordinaten
			glm::vec3(0, 0, 0),  // und schaut in den Ursprung
			glm::vec3(0, 1, 0)); // Oben ist bei (0,1,0), das ist die y-Achse = 1


// Modelmatrix : Hier auf Einheitsmatrix gesetzt, was bedeutet, dass die Objekte sich im Ursprung
// des Weltkoordinatensystems befinden.
		Model = glm::mat4(1.0f);
		Model = glm::rotate(Model, xWert, glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, yWert, glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, zWert, glm::vec3(0, 0, 1));

		glm::mat4 Save = Model;

		Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
		Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0)); //



		// Bind our texture in Texture Unit 0
	/*	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureMandrill);*/

		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);


		// Diese Informationen (Projection, View, Model) müssen geeignet der Grafikkarte übermittelt werden,
		// damit sie beim Zeichnen von Objekten berücksichtigt werden können.
		sendMVP();

		// Nachdem der GC in der Grafikkarte aktuell ist, also z. B. auch ein sendMVP ausgeführt wurde,
		// zeichen wir hier nun einen Würfel. Dazu werden in "drawWireCube" die Eckpunkte zur Grafikkarte 
		// geschickt. Der gewählte Modus legt fest, wie die Punkte mit Linien verbunden werden.
		// Das werden wir uns später noch genauer anschauen. (Schauen Sie sich die schwarzen Linien genau an,
		// und überlegen Sie sich, dass das wirklich ein Würfel ist, der perspektivisch verzerrt ist.)
		// Die Darstellung nennt man übrigens "im Drahtmodell".
		/* drawCube();
		 sendMVP();*/
		Model = Save;


		// Grundfläche erstellen
		drawGround(CubeFullTexure360);
		drawBackRound(CubeFullTexure360);
		drawZylinder(zylinderObj);
		drawTeeKanne(kanneObject, 4.0, 0.75, 0, kanneLinksYrotate);
		drawTeeKanne(kanneObject, -4.0, 0.75, 0, kanneRechtsYrotate);

		//Model = Save;
		//sendMVP();

		Model = Save;


		// mauer mit block Erstellen links ;
		drawWall(mauerArray, sizeof(mauerArray) / sizeof(mauerArray[0]));

		// mauer mit block Erstellen rechts ;
		drawWall(mauerArrayRechts, sizeof(mauerArrayRechts) / sizeof(mauerArrayRechts[0]));

		//Tuerme erstellen
		drawTower(turmArrayLinks, sizeof(turmArrayLinks) / sizeof(turmArrayLinks[0]));
		drawTower(turmArrayRechts, sizeof(turmArrayRechts) / sizeof(turmArrayRechts[0]));


		/*glBindVertexArray(VertexArrayIDQuader);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		sendMVP();*/
		Model = Save;
		Model = glm::rotate(Model, rXwert * rotate3, glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, rYwert * spin, glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, rZwert * roboWinkel, glm::vec3(0, 0, 1));
		drawSeg(1.5f);
		Model = glm::translate(Model, glm::vec3(0, 0, 1.5));
		Model = glm::rotate(Model, rotate2, glm::vec3(1, 0, 0));
		drawSeg(1.2f);
		Model = glm::translate(Model, glm::vec3(0, 0, 1.2));
		Model = glm::rotate(Model, trim, glm::vec3(1, 0, 0));

		drawSeg(0.8f);

		//drawSphere(10, 10);
		//drawBlock(zylinderObj);


		// Licht berechnen:
		enlightenScene();


		/*glm::vec4 lightPos = Model * glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y,lightPos.z);
		*/// Übergabe der Koordinaten
		/*flugobjektX = lightPos.x;
		flugobjektY = lightPos.y;
		flugobjektZ = lightPos.z;*/

		//Löschen der Visible  
		Model = Save;
		eraseVisibility(mauerArray, sizeof(mauerArray) / sizeof(mauerArray[0]));
		eraseVisibility(mauerArrayRechts, sizeof(mauerArrayRechts) / sizeof(mauerArrayRechts[0]));
		eraseVisibility(turmArrayRechts, sizeof(turmArrayRechts) / sizeof(turmArrayRechts[0]));
		eraseVisibility(turmArrayLinks, sizeof(turmArrayLinks) / sizeof(turmArrayLinks[0]));



		// Bildende. 
// Bilder werden in den Bildspeicher gezeichnet (so schnell wie es geht.). 
// Der Bildspeicher wird mit der eingestellten Bildwiederholfrequenz (also z. B. 60Hz)
// ausgelesen und auf dem Bildschirm dargestellt. Da beide Frequenzen nicht übereinstimmen, würde
// man beim Auslesen auf unfertige Bilder stoßen. Das wäre als Flimmern auf dem Bildschirm zu
// erkennen. (War bei älteren Grafikkarten tatsächlich so.)
// Dieses Problem vermeidet man, wenn man zwei Bildspeicher benutzt, wobei in einen gerade
// gemalt wird, bzw. dort ein neues Bild entsteht, und der andere auf dem Bildschirm ausgegeben wird.
// Ist man mit dem Erstellen eines Bildes fertig, tauscht man diese beiden Speicher einfach aus ("swap").
		glfwSwapBuffers(window);

		// Hier fordern wir glfw auf, Ereignisse zu behandeln. GLFW könnte hier z. B. feststellen,
		// das die Mouse bewegt wurde und eine Taste betätigt wurde.
		// Da wir zurzeit nur einen "key_callback" installiert haben, wird dann nur genau diese Funktion
		// aus "glfwPollEvents" heraus aufgerufen.
		glfwPollEvents();
	}


	glDeleteBuffers(1, &normalbuffer);
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);

	glDeleteBuffers(1, &uvbuffer);

	glDeleteTextures(1, &TextureRasen);
	glDeleteTextures(1, &TextureBergMitSee);
	glDeleteTextures(1, &TextureMauer);


	// Wenn der Benutzer, das Schliesskreusz oder die Escape-Taste betätigt hat, endet die Schleife und
	// wir kommen an diese Stelle. Hier können wir aufräumen, und z. B. das Shaderprogramm in der
	// Grafikkarte löschen. (Das macht zurnot das OS aber auch automatisch.)
	glDeleteProgram(programID);

	// Schießen des OpenGL-Fensters und beenden von GLFW.
	glfwTerminate();

	return 0; // Integer zurückgeben, weil main so definiert ist
}
