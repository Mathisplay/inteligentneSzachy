#include "chess.h"
#include <Adafruit_NeoPixel.h>

Chess c;

void setup(void)
{
  Serial.begin(230400);
  printBoard();
  //Adafruit_NeoPixel pixel = Adafruit_NeoPixel(64, 12, NEO_GRB + NEO_KHZ800);
}

void loop(void)
{
  String data = Serial.readString();
  if(data != "")
  {
    movingFrom(data);
  }
}

void printBoard()
{
  int** arr = c.printBoard();
  toFENandSend();
  for(int i = 0; i < 8; i++)
  {
    Serial.print(String(8 - i) + " ");
    for(int j = 0; j < 8; j++)
    {
      Serial.print(convertField(arr[i][j]) + " ");
    }
    Serial.print("\n");
  }
  Serial.print("  A  B  C  D  E  F  G  H\n");
}
String convertField(int field)
{
  switch(field)
  {
    case 0:
      return "[]";
    case 1:
      return "wP";
    case 2:
      return "wR";
    case 3:
      return "wN";
    case 4:
      return "wB";
    case 5:
      return "wQ";
    case 6:
      return "wK";
    case 7:
      return "bP";
    case 8:
      return "bR";
    case 9:
      return "bN";
    case 10:
      return "bB";
    case 11:
      return "bQ";
    case 12:
      return "bK";
  }    
}
String convertFieldFEN(int field)
{
  switch(field)
  {
    case 0:
      return " ";
    case 1:
      return "P";
    case 2:
      return "R";
    case 3:
      return "N";
    case 4:
      return "B";
    case 5:
      return "Q";
    case 6:
      return "K";
    case 7:
      return "p";
    case 8:
      return "r";
    case 9:
      return "n";
    case 10:
      return "b";
    case 11:
      return "q";
    case 12:
      return "k";
  }    
}
String movingFrom(String direct)
{
  int row1 = '8' - direct[1];
  int col1 = direct[0] - 'a';
  int row2 = '8' - direct[3];
  int col2 = direct[2] - 'a';
  int** arr = c.printBoard();
  int val1 = arr[row1][col1];
  int val2 = arr[row2][col2];
  String result = "";
  int typeOfMove = c.makeMove(row1, col1, row2, col2);
  if(typeOfMove != 0)
  {
    if(direct.length() > 4)
    {
      if(direct[4] == 'r')
      {
        if(c.turn == 0)
        {
          c.board[row2][col2] = 2;
        }
        else
        {
          c.board[row2][col2] = 8;
        }
      }
      else if(direct[4] == 'n')
      {
        if(c.turn == 0)
        {
          c.board[row2][col2] = 3;
        }
        else
        {
          c.board[row2][col2] = 9;
        }
      }
      else if(direct[4] == 'b')
      {
        if(c.turn == 0)
        {
          c.board[row2][col2] = 4;
        }
        else
        {
          c.board[row2][col2] = 10;
        }
      }
      else if(direct[4] == 'q')
      {
        if(c.turn == 0)
        {
          c.board[row2][col2] = 5;
        }
        else
        {
          c.board[row2][col2] = 11;
        }
      }
    }
    if(val1 != 0)
    {
      if(val1 > 6)
      {
        val1 -= 6;
      }
      switch(val1)
      {
        case 1:
          break;
        case 2:
          result += "R";
          break;
        case 3:
          result += "N";
          break;
        case 4:
          result += "B";
          break;
        case 5:
          result += "Q";
          break;
        case 6:
          result += "K";
          break;
        default:
          break;
      }
      switch(typeOfMove)
      {
        case 1:
          result += direct;
          break;
        case 2:
          result += direct[0];
          result += direct[1];
          result += "x";
          result += direct[2];
          result += direct[3];
          break;
        case 3:
          result += direct[0];
          result += direct[1];
          result += "x";
          result += direct[2];
          result += direct[3];
          result += "e.p.";
          break;
        case 4:
          result = "0-0-0";
          break;
        case 5:
          result = "0-0";
          break;
      }
      //Serial.println(result);
      c.turn = (c.turn + 1) % 2;
      printBoard();
      //toFENandSend();
      return result;
    }
    else
    {
      //Serial.println("Incorrect Move");
      return "Incorrect Move";
    }
  }
  //Serial.println("Incorrect Move");
  return "Incorrect Move";
}
void toFENandSend()
{
  int** arr = c.printBoard();
  String result = "";
  for(int i = 0; i < 8; i++)
  {
    int zeros = 0;
    for(int j = 0; j < 8; j++)
    {
      if(convertFieldFEN(arr[i][j]) == " ")
      {
        zeros++;
      }
      else
      {
        if(zeros > 0)
        {
          char ch = '0' + zeros;
          result += ch;
          zeros = 0;
        }
        result += convertFieldFEN(arr[i][j]);
      }
    }
    if(zeros > 0)
    {
      char ch = '0' + zeros;
      result += ch;
      zeros = 0;
    }
    if(i != 7)
    {
      result += "/";
    }
  }
  result += " ";
  if(c.turn == 0)
  {
    result += "w ";
  }
  else
  {
    result += "b ";
  }
  bool changed = false;
  if(c.castle[1] == true)
  {
    result += "K";
    changed = true;
  }
  if(c.castle[0] == true)
  {
    result += "Q";
    changed = true;
  }
  if(c.castle[3] == true)
  {
    result += "k";
    changed = true;
  }
  if(c.castle[2] == true)
  {
    result += "q";
    changed = true;
  }
  if(changed == false)
  {
    result += "-";
  }
  result += " ";
  if(c.enpassRow != -1)
  {
    switch(c.enpassColumn)
    {
      case 0:
        result += "a";
        break;
      case 1:
        result += "b";
        break;
      case 2:
        result += "c";
        break;
      case 3:
        result += "d";
        break;
      case 4:
        result += "e";
        break;
      case 5:
        result += "f";
        break;
      case 6:
        result += "g";
        break;
      case 7:
        result += "h";
        break;
    }
    if(c.enpassRow == 3)
    {
      result += "6";
    }
    else if(c.enpassRow == 4)
    {
      result += "3";
    }
    result += " 0 0";
  }
  else
  {
    result += "- 0 0";
  }
  delay(100);
  Serial.println("FEN "+ result);
}

