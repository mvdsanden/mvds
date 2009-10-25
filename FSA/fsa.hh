#ifndef __INC_MVDS_FSA_HH__
#define __INC_MVDS_FSA_HH__

namespace mvds {

  /** Finite State Automaton Class
   *
   */
  template <class Content, class StateKey>
  class FSA {
  public:

    // Forward declaration of nested class.
    class Node;

    template <class Iter>
    static std::shared_ptr<Node> walk(Iter begin, Iter end);

    template <class Iter, class Func>
    static std::shared_ptr<Node> walk(Iter begin, Iter end, Func func);

  };

  /** Finite State Automaton Node Class
   *
   *  Note: The use of shared_ptr is a problem, because of cyclidity.
   */
  template <class Content, class StateKey>
  class FSA<Content,StateKey>::Node {
  public:

    typedef std::multimap< StateKey,std::shared_ptr<Node> > container;

    typedef container::iterator iterator;

    typedef std::pair<iterator,iterator> iterator_pair;

	/**
	 *  @returns the content of the node.
	 */
    Content &content();

	/**
	 *  @returns the content of the node.
	 */
    Content const &content() const;

	/**
	 *  Add a state transition.
	 */
	void add(StateKey const &key, std::shared_ptr<Node> node);

	/**
	 *  Find a state transition with value key.
	 *  @returns an iterator to the next node (or end()).
	 */
    iterator find(StateKey const &key);

	/**
	 *  For multitple transitions with the same key, find
	 *  a range that is equal.
	 *  @returns a pair of iterators to the next nodes (or end()).
	 */
    iterator_pair equal_range(StateKey const &key);

	/**
	 *  @returns the start of the state transition list.
	 */
	iterator begin();

	/**
	 *  @returns the end of the state transition list.
	 */
	iterator end();

  private:

    container d_transitions;
    Content   d_content;

  };

  template <class Iter>
  inline std::shared_ptr<Node> FSA::walk(std::shared_ptr<Node> node,
										 Iter begin, Iter end)
  {
	for (Iter i = begin; i != end; ++i) {
	  Node::iterator j = node.find(*i);

	  if (j == node.end())
		return 0;

	  node = (*j).second;
	}

	return node;
  }

  template <class Iter, class Func>
  inline std::shared_ptr<Node> FSA::walk(std::shared_ptr<Node> node,
										 Iter begin, Iter end, Func func)
  {
	for (Iter i = begin; i != end; ++i) {
	  Node::iterator j = node.find(*i);

	  if (j == node.end())
		return 0;

	  node = (*j).second;

	  func(node);
	}

	return node;
  }

  template <class Content, class StateKey>
  inline Content &FSA<Content,StateKey>::Node::content()
  {
	return d_content;
  }

  template <class Content, class StateKey>
  inline Content const &FSA<Content,StateKey>::Node::content() const
  {
	return d_content;
  }

  template <class Content, class StateKey>
  inline void FSA<Content,StateKey>::Node::add(StateKey const &key, std::shared_ptr<Node> node)
  {
	d_transitions.insert(std::make_pair(key,node));
  }

  template <class Content, class StateKey>
  inline iterator FSA<Content,StateKey>::Node::find(StateKey const &key)
  {
	return d_transitions.find(key);
  }

  template <class Content, class StateKey>
  inline iterator_pair FSA<Content,StateKey>::Node::equal_range(StateKey const &key)
  {
	return d_transitions.equal_range(key);
  }

  template <class Content, class StateKey>
  inline iterator FSA<Content,StateKey>::Node::begin()
  {
	return d_transitions.begin();
  }

  template <class Content, class StateKey>
  inline iterator FSA<Content,StateKey>::Node::end()
  {
	return d_transitions.end();
  }


};

#endif // __INC_MVDS_FSA_HH__
