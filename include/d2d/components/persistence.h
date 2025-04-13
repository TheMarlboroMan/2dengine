#pragma once

#include "exception.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

namespace d2d { namespace components {

/**
 * safe persistence class. Stores groups of identifiers matched to a state.
 * Should identifiers of groups or nodes (or even states) be strings they
 * should not contain spaces, the equal sign or square brackets. Should custom 
 * types be used they should have their stream << operators overloaded.
 */
template<typename grouptype, typename idtype, typename statetype>
class persistence {

	public:

/**
 * returns the number of groups.
 */
	std::size_t                         size() const {

		return data.size();
	}

/**
 * returns the number of items in a group. Throws if the group does not
 * exist.
 */

	std::size_t                         size(grouptype _group) const {

		if(!this->has(_group)) {

			throw exception("cannot get size of non-existent persistence group");
		}

		return data.at(_group).size();
	}

/**
 * returns the count of members in a group that satisfy the given predicate.
 */
	std::size_t                         size(
		grouptype _group,
		std::function<bool(statetype)> _predicate
	) {

		if(!this->has(_group)) {

			throw exception("cannot get filtered size of non-existent persistence group");
		}

		return std::count_if(
			std::begin(data.at(_group)),
			std::end(data.at(_group)),
			[_predicate](const node& _node) -> bool {return _predicate(_node.state);}
		);
	}

/**
 * returns true if the group exists.
 */
	bool                                has(grouptype _group) const {

		return (bool) data.count(_group);
	}

/**
 * returns true if the group exists and has the given id within it.
 */
	bool                                has(grouptype _group, idtype _id) const {

		//remove constness from this to be able to call the non-const method.
		return nullptr!=const_cast<persistence<grouptype, idtype, statetype>*>
			(this)->find(_group, _id);
	}

/**
 * adds a group of the group does not exist. Throws if it exists.
 */
	void                                add(grouptype _group) {

		if(!this->has(_group)) {

			data.insert(std::make_pair(_group, list{}));
			return;
		}

		throw exception("cannot add already existing persistence group");
	}

/**
 * adds an item to the group. Throws if the group does not exist. Throws if
 * a node with the same id already exists.
 */
	void                                add(grouptype _group, idtype _id, statetype _state) {

		if(!this->has(_group)) {

			throw exception("cannot add node to non-existing persistence group");
		}

		auto found=this->find(_group, _id);
		if(nullptr!=found) {

			throw exception("cannot add existing node to persistence group");
		}

		data.at(_group).emplace_back(node{_id, _state});
	}

/**
 * sets the state of the item. Throws if the item or group do not exist.
 */
	void                                set(grouptype _group, idtype _id, statetype _state) {

		auto found=this->find(_group, _id);
		if(nullptr==found) {

			throw exception("cannot set the value to non-existing node in persistence group");
		}

		found->state=_state;
	}

/**
 * returns the state of the given item in the group. Throws if any of them 
 * don't exist.
 */
	statetype                            get(grouptype _group, idtype _id) const {

		auto found=const_cast<persistence<grouptype, idtype, statetype>*>
			(this)->find(_group, _id);
		if(nullptr==found) {

			throw exception("cannot retrieve state in non-existing node in persistence group");
		}

		return found->state;
	}
	
/**
 * returns true if the node in the group and id matches the given state.
 * Throws if the group or id do not exist.
 */
	bool                                is(grouptype _group, idtype _id, statetype _state) const {

		auto found=const_cast<persistence<grouptype, idtype, statetype>*>
			(this)->find(_group, _id);
		if(nullptr==found) {

			throw exception("cannot check state in non-existing node in persistence group");
		}

		return found->state==_state;
	}

/**
 * fully destroys the contents...
 */
	void                                clear() {

		data.clear();
	}

/**
 * cleans up contents, does not remove groups!
 */
	void                                reset() {

		for(auto& group : data) {

			group.second.clear();
		}
	}

/**
 * clears the given group if exists. Throws if it does not.
 */
	void                                clear(grouptype _group) {

		if(!this->has(_group)) {

			throw exception("cannot clear non-existent persistence group");
		}

		data.at(_group).clear();
	}

/**
 * removes the group if it exists. Throws if it does not.
 */
	void                                erase(grouptype _group) {

		if(!this->has(_group)) {

			throw exception("cannot erase non-existent persistence group");
		}

		data.erase(_group);
	}

/**
 * removes the item from the given group. Throws if any of them don't exist.
 */
	void                                erase(grouptype _group, idtype _id) {

		if(!this->has(_group)) {

			throw exception("cannot erase node from non-existent persistence group");
		}

		auto& l=data.at(_group);
		auto it=std::find_if(
			std::begin(l),
			std::end(l),
			[_id](const node _node) -> bool {return _node.id==_id;}
		);

		if(std::end(l)==it) {

			throw exception("cannot erase non-existent node from persistence group");
		}

		l.erase(it);
	}

/**
 * stores the persisted state into a regular text file. The state can be 
 * retrieved with "load_from_file".
 */
	void                                save_to_file(const std::string& _filename) {

		std::ofstream ofile{_filename};
		save(ofile);
	}

/**
 * retrieves a state previously persisted to a file.
 */
	void                                load_from_file(const std::string& _filename) {

		data.clear();
		std::ifstream ifile{_filename};
		load(ifile);
	}

/**
 * saves the state to a string that will be returned
 */
	std::string                         save_to_string() {

		std::stringstream ss;
		save(ss);
		return ss.str();
	}

/**
 * load state from the given string
 */
	void                                load_from_string(const std::string& _data) {

		std::stringstream ss(_data);
		load(ss);
	}

/**
 * generic save method, writes everything into a stream.
 */
	void                                save(std::ostream& _stream) {

		for(const auto& l : data) {

			_stream<<"["<<l.first<<"]"<<std::endl;

			if(l.second.size()) {

				for(const auto& piece : l.second) {

					_stream<<piece.id<<" = "<<piece.state<<" ";
				}
				//Remove last space...
				long pos=_stream.tellp();
				_stream.seekp(pos-1);
			}

			_stream<<std::endl;
		}
	}

/**
 * generic load method, loads this instance from a stream.
 */
	void                                load(std::istream& _stream) {

		data.clear();

		std::string line;
		grouptype groupname;
		std::istringstream iss;

		while(true) {

			std::getline(_stream, line);
			if(_stream.eof()) {

				break;
			}

			if('['==line[0]) {

				line.pop_back();
				line=line.substr(1);

				std::istringstream converter;
				converter.str(line);
				converter>>groupname;

				add(groupname);
				continue;
			}

			iss.str(line);
			if(!line.size()) {

				continue;
			}

			while(true) {

				idtype id;
				statetype state;

				iss>>id;
				iss.ignore(2, '=');
				iss>>state;

				add(groupname, id, state);

				if(iss.eof()) {

					break;
				}
			}

			iss.clear();
		}
	}

	private:

	struct node {
		idtype id;
		statetype state;
	};

	node *                              find(grouptype _group, idtype _id) {

		if(!this->has(_group)) {

			return nullptr;
		}

		auto& l=data.at(_group);
		auto it=std::find_if(
			std::begin(l),
			std::end(l),
			[_id](const node _node) -> bool {return _node.id==_id;}
		);

		if(std::end(l)==it) {

			return nullptr;
		}

		return &(*it);
	}

	typedef std::vector<node>           list; //list of identifiers.
	std::map<grouptype, list>           data;
};
}}
