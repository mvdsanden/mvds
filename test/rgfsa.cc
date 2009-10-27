#include <iostream>

#include "../FSA/fsa.hh"

using namespace mvds;
using namespace std;

typedef FSA<int,char>::Node Node;

int main(int argc, char *argv[])
{

  shared_ptr<Node> root = new Node;

  shared_ptr<Node> star = new Node;
  shared_ptr<Node> plus = new Node;
  shared_ptr<Node> dot = new Node;
  shared_ptr<Node> question = new Node;
  shared_ptr<Node> disj = new Node;
  shared_ptr<Node> range = new Node;
  shared_ptr<Node> parentRoot = new Node;
  shared_ptr<Node> parentDisj = new Node;
  shared_ptr<Node> parentRange = new Node;
  shared_ptr<Node> done = new Node(1);

  for (char i = 'a'; i <= 'z'; ++i)
    root->add(i,root);

  for (char i = 'A'; i <= 'Z'; ++i)
    root->add(i,root);

  root->add('*',star);
  root->add('+',plus);
  root->add('.',dot);
  root->add('?',question);
  root->add('[',disj);
  root->add('(',parentRoot);

  root->add(0,done);

  disj->add(']',root);

  for (char i = 'a'; i <= 'z'; ++i)
    disj->add(i,disj);

  for (char i = 'A'; i <= 'Z'; ++i)
    disj->add(i,disj);

  disj->add('-',range);

  disj->add('(',parentDisj);

  for (char i = 'a'; i <= 'z'; ++i)
    range->add(i,disj);

  for (char i = 'A'; i <= 'Z'; ++i)
    range->add(i,disj);

  range->add('(',parentRange);

  parentRoot->add(')',root);
  parentDisj->add(')',disj);
  parentRange->add(')',disj);

  for (char i = 'a'; i <= 'z'; ++i) {
    parentRoot->add(i,parentRoot);
    parentDisj->add(i,parentDisj);
    parentRange->add(i,parentRange);
  }

  for (char i = 'A'; i <= 'Z'; ++i) {
    parentRoot->add(i,parentRoot);
    parentDisj->add(i,parentDisj);
    parentRange->add(i,parentRange);
  }



  return 0;
}
