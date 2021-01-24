#ifndef SONODE_H
#define SONODE_H

class SONode{
public:
  int s;
  int o;
  SONode* s_before;
  SONode* s_after;
  SONode* o_before;
  SONode* o_after;
  SONode(int s, int o);
};

#endif
