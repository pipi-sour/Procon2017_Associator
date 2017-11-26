#include <algorithm> //sort()
#include <cstdlib> //abs(), stoi()
#include <fstream> //ifstream()
#include <iostream> //cin, cout, endl
#include <string> //string
#include <tuple> //pair, tuple
#include <vector> //vector
#include "Puzzle.hpp"

using namespace std;

typedef struct shape_t {
  vector<pair<int, int> > vertex; //頂点情報
  vector<double> angle; //角度情報
  vector<double> edge; //辺長情報
  double perim; //周長情報
  double surface; //面積情報
	
  shape_t() {
    perim = 0.0;
    surface = 0.0;
  }
	
} shape_t;

//頂点情報が格納されたテキストファイルから頂点を抽出
vector<vector<pair<int, int> > > DataReader(string dir, string mode);

//頂点情報を分析し角度、辺長、面積を算出、shape_tに格納
vector<shape_t> DataAnalyzer(vector<vector<pair<int, int> > > vertexList);

//理論値、計算値のピースのペアを探す
vector<vector<tuple<int, int ,double> > > PairFinder(vector<shape_t> exPiece, vector<shape_t> clPiece, const int tolVertex, const float tolPerimeter, const float tolSurface, const int tolMaxAngle, const int tolMinAngle);

//理論値のピース番号順に並べ替えて結果をHTMLで出力
void HTMLGenerator(string dir, vector<vector<tuple<int, int ,double> > > evalList, const int pieceNum);

//組内の評価値を降順（高い順）に並べる
bool PointsSorter_Descending(tuple<int, int, double> t1, tuple<int, int, double> t2);

//組内の理論値のピース番号を昇順（低い順）に並べる
bool NumberSorter_Ascending(tuple<int, int, double> t1, tuple<int, int, double> t2);


int main(int argc, const char * argv[]) {
	
  const char *dir_ = argv[1];
  string dir = string(dir_);
	
  if(dir.back() != '/') {
    dir.push_back('/');
  }
	
  //理論値、計算値のテキストファイルから頂点情報を格納
  vector<vector<pair<int, int> > > expc = DataReader(dir, "理論値");
  vector<vector<pair<int, int> > > calc = DataReader(dir, "計算値");
	
  //理論値、計算値それぞれの各ピース情報を分析
  vector<shape_t> ex = DataAnalyzer(expc);
  vector<shape_t> cl = DataAnalyzer(calc);
	
  //Severe Mode（Defaultより厳しめな基準、似たピースが多い場合に最適
  //result_t interim = PairFinder(ex, cl, 1, float(0.2), float(0.2), 45, 45);
	
  //Default Mode
  vector<vector<tuple<int, int ,double> > > evalList = PairFinder(ex, cl, 3, float(0.3), float(0.3), 60, 60);
	
  HTMLGenerator(dir, evalList, int(expc.size()));
	
  cout << "Completed!" << endl << endl;
    
  return 0;
}

vector<vector<pair<int, int> > > DataReader(string dir, string mode) {
  string buf = ""; //読み込んだ文字列
  int vnum, tmpX, tmpY, i = 0; //読み込んだ頂点数、座標点、while文
  vector<pair<int, int> > vtmp; //ピース毎の座標点
  vector<vector<pair<int, int> > > vertexList; //頂点リスト
	
  ifstream ifs(dir + mode + "/ピース.txt", ios::in);
  
  if(!ifs) {
    cerr << "\"" << dir << mode << "/ピース.txt" << "\" was not found(404)." << endl << endl;;
    exit(0);
  } else {
    cout << "\"" << dir << mode << "/ピース.txt" << "\" was found!" << endl << endl;
  }

  //1行先に進める
  getline(ifs, buf);
	
  while(getline(ifs, buf)) {
    vtmp.clear();
    //頂点数を格納
    vnum = stoi(buf);
    //ピースの頂点数分繰り返す
    for(int j = 0; j < vnum; j++) {
      //1行先に進める
      getline(ifs, buf);
      sscanf(buf.data(), "%d %d", &tmpX, &tmpY);
      vtmp.push_back(make_pair(tmpX, tmpY));
    }
    //座標をを頂点リストに返す
    vertexList.push_back(vtmp);
    i++;
  }
  return vertexList; //頂点リストを返す
}

vector<shape_t> DataAnalyzer(vector<vector<pair<int, int> > > vertexList) {
  cout << "Analyzing Data..." << endl << endl;
  shape_t piece; //ピースごとの情報を格納
  vector<shape_t> pieceList; //すべてのピースごとの情報を格納
    double atmp = 0.0;
    double etmp = 0.0;
    bool deleteFlag = false;
	
  for(int i = 0; i < vertexList.size(); i++) {
    //頂点、角度、辺長、面積の一時データを削除
    piece.vertex.clear();
    piece.angle.clear();
    piece.edge.clear();
    piece.perim = 0.0;
    piece.surface = 0.0;
	  
    //cout << "No. " << i << endl;
	  
    for(int j = 0; j < vertexList[i].size(); j++) {
		
      if(deleteFlag == false) {
        //頂点が削除対象でなかったらそのまま格納
        piece.vertex.push_back(make_pair(vertexList[i][j].first, vertexList[i][j].second));
        //cout << '(' << piece.vertex[j].first << ", "  << piece.vertex[j].second << ')';
      } else {
        //頂点が削除対象であったら頂点は格納せずフラグを下ろす
        deleteFlag = false;
      }
		
      //角度を一時的に算出
      atmp = Puzzle::calcAngle(vertexList[i][(j+1)%vertexList[i].size()].first, vertexList[i][(j+1)%vertexList[i].size()].second, vertexList[i][(j+2)%vertexList[i].size()].first, vertexList[i][(j+2)%vertexList[i].size()].second, vertexList[i][j].first, vertexList[i][j].second);
      
      //角度(angle)、辺長(edge)を算出
      if(atmp > 177 && atmp < 183) {
        
        //頂点がほぼ直線上にある場合はフラグを上げ、一時的に辺長をetmpに加算
        deleteFlag = true;
        etmp += Puzzle::calcSide(vertexList[i][j].first, vertexList[i][j].second, vertexList[i][(j+1)%vertexList[i].size()].first, vertexList[i][(j+1)%vertexList[i].size()].second);
        //cout << "This vertex was deleted." << endl;
        
      } else {
        //そうでない場合は通常通り角度を格納
        piece.angle.push_back(atmp);
        //前回までにetmpが溜まっていたら現在の辺長と足し合わせて格納、etmpはリセット
        piece.edge.push_back(Puzzle::calcSide(vertexList[i][j].first, vertexList[i][j].second, vertexList[i][(j+1)%vertexList[i].size()].first, vertexList[i][(j+1)%vertexList[i].size()].second) + etmp);
        etmp = 0.0;
        //cout << " Angle: " << piece.angle[j] << "  Edge: " << piece.edge[j] << "  " << endl;
      }
		
      //最後の頂点が誤検出された頂点となってしまった場合はいろいろと調整
      if(deleteFlag == true && (int)vertexList[i].size() == j+1) {
        piece.vertex[0] = piece.vertex[j];
        piece.edge[0] += etmp;
        etmp = 0.0;
        piece.vertex.pop_back();
      }

      //周長(perim)を算出
      piece.perim += Puzzle::calcSide(vertexList[i][j].first, vertexList[i][j].second, vertexList[i][(j+1)%vertexList[i].size()].first, vertexList[i][(j+1)%vertexList[i].size()].second);
		
      //面積(surface)の2倍を算出
      piece.surface += vertexList[i][j].second * (vertexList[i][(j-1+vertexList.at(i).size())%vertexList.at(i).size()].first - vertexList[i][(j+1)%vertexList.at(i).size()].first);
		
    }
    //2で割って面積とする
    piece.surface *= .5;
	  
    sort(piece.angle.begin(), piece.angle.end());
    sort(piece.edge.begin(), piece.edge.end());
    //cout << "-> Perimeter: " << piece.perim << endl;
    //cout << "-> Surface: " << piece.surface << endl << endl;
	  
    pieceList.push_back(piece); //ピースをピースリストに追加
  }
  return pieceList; //でき上がったピースリストを返す
}

vector<vector<tuple<int, int ,double> > > PairFinder(vector<shape_t> exPiece, vector<shape_t> clPiece, const int tolVertex, const float tolPerimeter, const float tolSurface, const int tolMaxAngle, const int tolMinAngle) {
  
  cout << "Finding Pairs..." << endl << endl;
  
  vector<pair<int, int> > candList; //候補リスト
  vector<pair<int, int> > candListC; //各ピースの候補数
  int tmp; //候補数を格納
	
  for(int i = 0; i < exPiece.size(); i++) {
    tmp = 0;
    for(int j = 0; j < clPiece.size(); j++) {

      //頂点数差が*以下、かつ周長差と面積差が理論値の*倍以下であれば
      if((abs(int(exPiece[i].vertex.size()) - int(clPiece[j].vertex.size())) <= tolVertex) && (abs(int(exPiece[i].perim) - int(clPiece[j].perim)) <= exPiece[i].perim * tolPerimeter) && (abs(int(exPiece[i].surface) - int(clPiece[j].surface)) <= exPiece[i].surface * tolSurface)) {
		  
        //理論値・計算値の角度の最大値の差が**度以内、あるいは最小値の差が**度以内であれば
        if((abs(*max_element(exPiece[i].angle.begin(), exPiece[i].angle.end()) - *max_element(clPiece[j].angle.begin(), clPiece[j].angle.end())) <= tolMaxAngle) && (abs(*min_element(exPiece[i].angle.begin(), exPiece[i].angle.end()) - *min_element(clPiece[j].angle.begin(), clPiece[j].angle.end())) <= tolMinAngle)) {
			
          candList.push_back(make_pair(i, j));
          tmp++;
          //cout << "候補リスト: " << i << "," << j << endl;
			
        }
      }
    }
    candListC.push_back(make_pair(tmp, i));
	  
  }
	
  sort(candListC.begin(), candListC.end());

  vector<double> difA, difE, difP, difS; //角度差、辺長差、周長差、面積差(difference + (angle)(edge)(perimeter)(surface))
  double tdifA, tdifE; //角度差、辺長差の一時データ(tmp + difference)
  bool flag; //添字調整用
  tmp = 0; //添字調整用
	
  vector<tuple<int, int, double> > eval; //評価値(evaluation)
  vector<vector<tuple<int, int ,double> > > evalList;
	
  for(int h = 0; h < exPiece.size(); h++) {
	  
    difA.clear();
    difE.clear();
    difP.clear();
    difS.clear();
    eval.clear();
    flag = false;
	  
    for(int i = 0; i < candList.size(); i++) {
		
      if(candListC[h].second == candList[i].first) {
		  
        //候補となるピースの添字を格納
        if(flag == false) {
          tmp = i;
          flag = true;
        }
		  
        tdifA = 0.0; tdifE = 0.0;
		  
        for(int j = 0; j < exPiece[candList[i].first].vertex.size(); j++) {
			
          tdifA += abs(exPiece[candList[i].first].angle[j] - clPiece[candList[i].second].angle[j])/exPiece[candList[i].first].angle[j];
			
          //cout << "(" << exPiece[candidateList[i].first].angle[j] << "-" << clPiece[candidateList[i].second].angle[j] << ")/" << exPiece[candidateList[i].first].angle[j] << " = " << tdifA << endl;
			
          tdifE += abs(exPiece[candList[i].first].edge[j] - clPiece[candList[i].second].edge[j])/exPiece[candList[i].first].edge[j];
			
          //cout << "tdifA: " << tdifA << " tdifE: " << tdifE << endl;
			
        }
		  
        difA.push_back(1-(tdifA/exPiece[candList[i].first].vertex.size()));
		  
        difE.push_back(1-(tdifE/exPiece[candList[i].first].vertex.size()));
		  
        difP.push_back(1-abs((exPiece[candList[i].first].perim - clPiece[candList[i].second].perim)/exPiece[candList[i].first].perim));
		  
        difS.push_back(1-abs((exPiece[candList[i].first].surface - clPiece[candList[i].second].surface)/exPiece[candList[i].first].surface));
		  
        //cout << '[' << candList[i].first << ']' << '[' << candList[i].second << ']' << endl;
        //cout << "->ANGLE: " << difA[i-tmp] << endl << "->EDGE: " << difE[i-tmp] << endl << "->PERIMETER: " << difP[i-tmp] << endl << "->SURFACE: " << difS[i-tmp] << endl << endl;
		  
        eval.push_back(make_tuple(candList[i].first, candList[i].second, (difA[i-tmp]*0.2 + difE[i-tmp]*0.2 + difP[i-tmp]*2.0 + difS[i-tmp]*1.6)/4));
        //eval.push_back(make_tuple(candList[i].first, candList[i].second, (difA[i-tmp] + difE[i-tmp] + difP[i-tmp] + difS[i-tmp])/4));
      }
    }
	  
    if(eval.size() <= 5) {
      const int x = 5 - (int)eval.size();
      for(int i = 0; i < x; i++) {
        eval.push_back(make_tuple(candListC[h].second, -1, -1.0));
      }
    }
	  
    //評価値の高い順に並べ替え
    sort(eval.begin(), eval.end(), PointsSorter_Descending);
	  
    //評価値を評価値リストに追加
    evalList.push_back(eval);
  }
  return evalList;
}

void HTMLGenerator(string dir, vector<vector<tuple<int, int ,double> > > evalList, const int pieceNum) {
	
  //ベスト5をそれぞれ宣言
  vector<tuple<int, int, double> > highest;
  vector<tuple<int, int, double> > secondHighest;
  vector<tuple<int, int, double> > thirdHighest;
  vector<tuple<int, int, double> > forthHighest;
  vector<tuple<int, int, double> > fifthHighest;
	
  //ベスト5をそれぞれ格納
  for(int i = 0; i < pieceNum; i++) {
    highest.push_back(evalList[i][0]);
    secondHighest.push_back(evalList[i][1]);
    thirdHighest.push_back(evalList[i][2]);
    forthHighest.push_back(evalList[i][3]);
    fifthHighest.push_back(evalList[i][4]);
  }

  //ベスト5をそれぞれ並べ替え
  sort(highest.begin(), highest.end(), NumberSorter_Ascending);
  sort(secondHighest.begin(), secondHighest.end(), NumberSorter_Ascending);
  sort(thirdHighest.begin(), thirdHighest.end(), NumberSorter_Ascending);
  sort(forthHighest.begin(), forthHighest.end(), NumberSorter_Ascending);
  sort(fifthHighest.begin(), fifthHighest.end(), NumberSorter_Ascending);

  cout << "Generating HTML File..." << endl << endl;
	
  ofstream html(dir + "Associating Result.html");
	
  html << "<!doctype html><html> <head> <meta charset=\"UTF-8\"> <title>Associating Result</title> <link rel=\"stylesheet\" href=\"main.css\" type=\"text/css\"> </head> <body> <header>Associating Result</header> <main> <div class=\"table-wrapper\" id=\"solution\"> <table id=\"solution-table\"> <thead> <tr> <th>No.</th> <th colspan=\"5\">BEST 5</th> </tr></thead> <tbody>";
	
  for(int i = 0; i < pieceNum; i++) {
    html << "<tr id=\"piece" << i <<  "\" onclick=\"cellClick(" << i << ")\")>";
    html << "<th class=\"expNumber\"><span>" << i << "</span></th>";
    html << "<td class=\"result\"><span class=\"pNumber p" << get<1>(highest[i]) << "\">" << get<1>(highest[i]) << "</span><br><span class=\"points\">" << get<2>(highest[i])*100 << "</span></td>";
    html << "<td class=\"result\"><span class=\"pNumber p" << get<1>(secondHighest[i]) << "\">" << get<1>(secondHighest[i]) << "</span><br><span class=\"points\">" << get<2>(secondHighest[i])*100 << "</span></td>";
    html << "<td class=\"result\"><span class=\"pNumber p" << get<1>(thirdHighest[i]) << "\">" << get<1>(thirdHighest[i]) << "</span><br><span class=\"points\">" << get<2>(thirdHighest[i])*100 << "</span></td>";
    html << "<td class=\"result\"><span class=\"pNumber p" << get<1>(forthHighest[i]) << "\">" << get<1>(forthHighest[i]) << "</span><br><span class=\"points\">" << get<2>(forthHighest[i])*100 << "</span></td>";
    html << "<td class=\"result\"><span class=\"pNumber p" << get<1>(fifthHighest[i]) << "\">" << get<1>(fifthHighest[i]) << "</span><br><span class=\"points\">" << get<2>(fifthHighest[i])*100 << "</span></td>";
    html << "</tr>";
  }

  html << "</tbody> </table> </div><div class=\"table-wrapper\" id=\"hiding\"> <table id=\"color-code\"> <tr> <th id=\"BL\">Blue</th> <td>100 ~ 99</td></tr><tr> <th id=\"CY\">Cyan</th> <td>98 ~ 96</td></tr><tr> <th id=\"TL\">Teal</th> <td>95 ~ 93</td></tr><tr> <th id=\"GR\">Green</th> <td>92 ~ 90</td></tr><tr> <th id=\"LG\">Light Green</th> <td>89 ~ 85</td></tr><tr> <th id=\"YL\">Yellow</th> <td>85 ~ 75</td></tr><tr> <th id=\"DO\">Deep Orange</th> <td>75 ~ 60</td></tr><tr> <th id=\"RD\">Red</th> <td>59 ~ 0</td></tr></table> <table id=\"hiding-table\">";
	
  for(int i = 0; i < pieceNum; i++) {
    if(i % 10 == 0) {
      html << "<tr>";
    }
    html << "<td onClick=\"cellHiding(\'p" << i << "\')\"><span class=\"p" << i << "h\">" << i << "</span></td>";
    if(i % 10 == 9) {
      html << "</tr>";
    }
  }
	
  html << "</table> </div></main> <script src=\"script.js\"></script> </body></html>" << endl;
	
  html.close();
}

bool PointsSorter_Descending(tuple<int, int, double> t1, tuple<int, int, double> t2) {
  return get<2>(t1) > get<2>(t2);
}

bool NumberSorter_Ascending(tuple<int, int, double> t1, tuple<int, int, double> t2) {
  return get<0>(t1) < get<0>(t2);
}
