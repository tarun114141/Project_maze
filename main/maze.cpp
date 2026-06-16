#include<iostream>
#include<GL/glut.h>
#include<vector>
#include<stack>
#include<queue>
#include"D:\animations\Extra_lib\Button.h"
#include"D:\animations\Extra_lib\SimpleRecorder.h"
using namespace std;


#define WIDTH  800.0
#define HEIGHT 700.0


//states++++++++++++++++++++++++++++++++++++++++++++++++++++
enum State { IDLE, GENERATING, GENERATED, SOLVING, SOLVED };
State appState = IDLE;


class cell{
    private:
    bool visited;
    bool top;
    bool bottom;
    bool right;
    bool left;
    bool expolered;

    public:

    cell(){
        visited=expolered=false;
        top=bottom=right=left=true;
    }

    void setvisited(bool v){
        visited= v;
    }
    void set_expolered(bool v){
        expolered= v;
    }

    bool is_expolered(){
        return expolered;
    }

    bool hasVisited(){
        return visited;
    }
    bool has_top(){
        return top;
    }
    bool has_left(){
        return left;
    }
    bool has_right(){
        return right;
    }
    bool has_bottom(){
        return bottom;
    }

    void r_top(){
        top= false;
    }
    void r_left(){
        left = false;
    }
    void r_right(){
        right= false;
    }
    void r_bottom(){
        bottom= false;
    }

};



class Maze{

    private:
    int r;
    int c;
    int path_index;

    stack<pair<int,int>> dfs_stack;
    queue<pair<int,int>> q;

    int curr_row;
    int curr_col;
    vector<vector<cell>> cells;
    vector<pair<int,int>> sol_path;
    vector<vector<bool>> explored;
    vector<vector<pair<int,int>>> parent;

    public:
    Maze(int r, int c){
        this->r=r;
        this->c=c;
        this->path_index=0;
        this->curr_row=0;
        this->curr_col=0;
        cells.resize(r, vector<cell>(c));
        explored.resize(r,vector<bool>(c,false));
        parent.resize(r,vector<pair<int,int>>(c,{-1,-1}));
    }

    void reset(){
        cells.assign(r, vector<cell>(c));
        explored.assign(r,vector<bool>(c,false));
        parent.assign(r,vector<pair<int,int>>(c,{-1,-1}));
        while(!dfs_stack.empty()) dfs_stack.pop();
        while(!q.empty()) q.pop();
        sol_path.clear();
        curr_row=0;
        curr_col=0;
        path_index=0;
    }

    void startGeneration(){
        reset();
    }

    void startSolving(){
        q.push({0,0});
        explored[0][0]=true;
        curr_row=0;
        curr_col=0;
    }


    //neighbours++++++++++++++++++++++++++++++++++++++++++++++++++++

    vector<pair<int,int>> getAcceblNeighbour(int r, int c){
        vector<pair<int,int>> n;

        if(r > 0 && !cells[r][c].has_top()){
            n.push_back({r-1,c});
        }
        if(r < this->r-1 && !cells[r][c].has_bottom()){
            n.push_back({r+1,c});
        }
        if(c > 0 && !cells[r][c].has_left()){
            n.push_back({r,c-1});
        }
        if(c < this->c-1 && !cells[r][c].has_right()){
            n.push_back({r,c+1});
        }

        return n;
    }


    vector<pair<int,int>> get_Neighbour(int r, int c){

        vector<pair<int,int>> neighbour;

        if(r > 0 && !cells[r-1][c].hasVisited()){
            neighbour.push_back({r-1,c});
        }
        if(r < (int)cells.size()-1 && !cells[r+1][c].hasVisited()){
            neighbour.push_back({r+1,c});
        }
        if(c > 0 && !cells[r][c-1].hasVisited()){
            neighbour.push_back({r,c-1});
        }
        if(c < (int)cells[0].size()-1 && !cells[r][c+1].hasVisited()){
            neighbour.push_back({r,c+1});
        }

        return neighbour;
    }


    void removeWall(int r1, int c1, int r2, int c2){

        if(r1==r2){
            if(c1 < c2){
                cells[r1][c1].r_right();
                cells[r2][c2].r_left();
            }else{
                cells[r1][c1].r_left();
                cells[r2][c2].r_right();
            }
        }

        if(c1==c2){
            if(r1<r2){
                cells[r1][c1].r_bottom();
                cells[r2][c2].r_top();
            }else{
                cells[r1][c1].r_top();
                cells[r2][c2].r_bottom();
            }
        }
    }


    //update++++++++++++++++++++++++++++++++++++++++++++++++++++

    void update(){

        switch(appState){

            case IDLE:
            case GENERATED:
                return;

            case GENERATING:{

                cells[curr_row][curr_col].setvisited(true);

                vector<pair<int,int>> neighbour= get_Neighbour(curr_row,curr_col);

                if(!neighbour.empty()){
                    int random_index = rand() % neighbour.size();
                    int next_row = neighbour[random_index].first;
                    int next_col = neighbour[random_index].second;

                    dfs_stack.push({curr_row,curr_col});
                    removeWall(curr_row,curr_col,next_row,next_col);
                    curr_row=next_row;
                    curr_col=next_col;

                }else if(!dfs_stack.empty()){
                    curr_row=dfs_stack.top().first;
                    curr_col=dfs_stack.top().second;
                    dfs_stack.pop();

                }else{
                    curr_row=0;
                    curr_col=0;
                    appState=GENERATED;
                }

                break;
            }

            case SOLVING:{

                if(q.empty()) return;

                auto current= q.front();
                q.pop();

                curr_row=current.first;
                curr_col=current.second;

                cells[curr_row][curr_col].set_expolered(true);

                if(curr_row == r-1 && curr_col == c-1){
                    int r1 = curr_row;
                    int c1 = curr_col;

                    while(!(r1 == 0 && c1 == 0)){
                        sol_path.push_back({r1,c1});
                        auto p = parent[r1][c1];
                        r1 = p.first;
                        c1 = p.second;
                    }

                    sol_path.push_back({0,0});
                    appState=SOLVED;
                    return;
                }

                vector<pair<int,int>> n= getAcceblNeighbour(curr_row,curr_col);
                for(auto next: n){
                    int n_r=next.first;
                    int n_c=next.second;
                    if(!explored[n_r][n_c]){
                        explored[n_r][n_c]=true;
                        parent[n_r][n_c]={curr_row,curr_col};
                        q.push({n_r,n_c});
                    }
                }

                break;
            }

            case SOLVED:{
                if(path_index < (int)sol_path.size()){
                    path_index++;
                }
                break;
            }
        }
    }


    //draw++++++++++++++++++++++++++++++++++++++++++++++++++++

    void draw(){

        glViewport(0, 0, int(0.8*WIDTH), int(HEIGHT));

        float cell_width= 1.96f/ c;
        float cell_height= 1.96f/r;

        float padding_x = cell_width * 0.25f;
        float padding_y = cell_height * 0.25f;

        for(int i=0;i<r;i++){
            for(int j=0;j<c;j++){
                float x=-0.98 + j* cell_width;
                float y=0.98 - i* cell_height;

                if(cells[i][j].hasVisited()){
                    glColor3f(0.2f,0.2f,0.2f);
                    glBegin(GL_QUADS);
                    glVertex2f(x,y);
                    glVertex2f(x+cell_width,y);
                    glVertex2f(x+cell_width,y-cell_height);
                    glVertex2f(x,y-cell_height);
                    glEnd();
                }

                if(cells[i][j].is_expolered()){
                    glColor3f(0.0f,0.0f,0.3f);
                    glBegin(GL_QUADS);
                    glVertex2f(x,y);
                    glVertex2f(x+cell_width,y);
                    glVertex2f(x+cell_width,y-cell_height);
                    glVertex2f(x,y-cell_height);
                    glEnd();
                }

                if(i==curr_row && j==curr_col){
                    glColor3f(0.0f,1.0f,0.0f);
                    glBegin(GL_QUADS);
                    glVertex2f(x,y);
                    glVertex2f(x+cell_width,y);
                    glVertex2f(x+cell_width,y-cell_height);
                    glVertex2f(x,y-cell_height);
                    glEnd();
                }

                for(int k=0;k<path_index && k<(int)sol_path.size();k++){
                    int row = sol_path[k].first;
                    int col = sol_path[k].second;

                    if(i==row && j==col){
                        glColor3f(1.0f,0.0f,0.0f);
                        glBegin(GL_QUADS);
                        glVertex2f(x+padding_x,y-padding_y);
                        glVertex2f(x+cell_width-padding_x,y-padding_y);
                        glVertex2f(x+cell_width-padding_x,y-cell_height+padding_y);
                        glVertex2f(x+padding_x,y-cell_height+padding_y);
                        glEnd();
                    }
                }

                glColor3f(0,1,0);
                glBegin(GL_LINES);

                if(cells[i][j].has_top()){
                    glVertex2f(x,y);
                    glVertex2f(x+cell_width,y);
                }
                if(cells[i][j].has_bottom()){
                    glVertex2f(x,y-cell_height);
                    glVertex2f(x+cell_width,y-cell_height);
                }
                if(cells[i][j].has_left()){
                    glVertex2f(x,y);
                    glVertex2f(x, y-cell_height);
                }
                if(cells[i][j].has_right()){
                    glVertex2f(x+cell_width,y);
                    glVertex2f(x+cell_width, y-cell_height);
                }

                glEnd();
            }
        }
    }

};


//globals++++++++++++++++++++++++++++++++++++++++++++++++++++

Maze maze1(20,20);

Button btnCreate(-0.9f, 0.8f, 1.8f, 0.15f, "Create Maze");
Button btnSolve (-0.9f, 0.5f, 1.8f, 0.15f, "Solve");
Button btnReset (-0.9f, 0.2f, 1.8f, 0.15f, "Reset");


//coordinates++++++++++++++++++++++++++++++++++++++++++++++++++++

float GL_x(int x){ 
    return (x / (WIDTH/ 2.0f)) - 1.0f; 
}
float GL_y(int y){ 
    return 1.0f - (y / (HEIGHT/ 2.0f)); 
}

float panel_x(int x){ 
    return ((x - 0.8f * WIDTH) / (0.1f * WIDTH)) - 1.0f; 
}


//status text++++++++++++++++++++++++++++++++++++++++++++++++++++

void drawStatusText(const char* text){
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(-0.8f,-0.5f);
    for(const char* ch = text; *ch; ch++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ch);
    }
}


//display++++++++++++++++++++++++++++++++++++++++++++++++++++

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    maze1.draw();

    glViewport(int(0.8*WIDTH), 0, int(0.2*WIDTH), int(HEIGHT));

    btnCreate.setFont(GLUT_BITMAP_HELVETICA_12);
    btnSolve.setFont(GLUT_BITMAP_HELVETICA_12);
    btnReset.setFont(GLUT_BITMAP_HELVETICA_12);

    if(appState == IDLE){
        btnCreate.setNormalColor(0.2f,0.5f,0.2f);
    }else{
        btnCreate.setNormalColor(0.1f,0.2f,0.1f);
    }

    if(appState == GENERATED){
        btnSolve.setNormalColor(0.2f,0.2f,0.5f);
    }else{
        btnSolve.setNormalColor(0.1f,0.1f,0.2f);
    }

    btnReset.setNormalColor(0.5f,0.1f,0.1f);

    btnCreate.draw();
    btnSolve.draw();
    btnReset.draw();

    switch(appState){
        case IDLE:       
            drawStatusText("Press Create Maze");
            break;
        case GENERATING: 
            drawStatusText("Generating...");
                 break;
        case GENERATED: 
            drawStatusText("Press Solve");       
            break;
        case SOLVING:    
            drawStatusText("Solving...");
            break;
        case SOLVED:
             drawStatusText("Solved!");
            break;
    }

    glutSwapBuffers();
}


//timer++++++++++++++++++++++++++++++++++++++++++++++++++++

void timer(int value){
    maze1.update();
    saveAnimationFrame(WIDTH,HEIGHT);
    glutPostRedisplay();
    glutTimerFunc(32,timer,0);
}


//mouse++++++++++++++++++++++++++++++++++++++++++++++++++++

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        float px= panel_x(x);
        float py= GL_y(y);

        if(btnCreate.isClicked(px,py)){
            if(appState == IDLE){
                maze1.startGeneration();
                appState=GENERATING;
            }
        }

        if(btnSolve.isClicked(px,py)){
            if(appState == GENERATED){
                maze1.startSolving();
                appState=SOLVING;
            }
        }

        if(btnReset.isClicked(px,py)){
            maze1.reset();
            appState=IDLE;
        }
    }
}


//hover++++++++++++++++++++++++++++++++++++++++++++++++++++

void motion(int x, int y){
    float px= panel_x(x);
    float py= GL_y(y);
    btnCreate.updateHover(px,py);
    btnSolve.updateHover(px,py);
    btnReset.updateHover(px,py);
    glutPostRedisplay();
}


//main++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("Random maze generator");

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(32,timer,0);
    glutMainLoop();
    return 0;
}