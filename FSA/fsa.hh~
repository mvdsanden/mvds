#ifndef __INC_MVDS_FSA_HH__
#define __INC_MVDS_FSA_HH__

namespace mvds {

  template <class Content, class StateKey>
  class FSA {
  public:

    // Forward declaration of nested class.
    class Node;

    template <class Iter>
    static std::shared_ptr<Node> walk(Iter begin, Iter end);

  };

  template <class Content, class StateKey>
  class FSA::Node {
  public:

    typedef std::multimap< StateKey,std::shared_ptr<Node> > container;

    typedef container::iterator iterator;

    typedef std::pair<iterator,iterator> iterator_pair;

    Content &content();

    Content const &content() const;

    iterator find(StateKey const &key);

    iterator_pair equal_range(StateKey const &key);

  private:

    container d_transitions;

    Content   d_content;

  };

};

#endif // __INC_MVDS_FSA_HH__
