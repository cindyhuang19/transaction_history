#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//

Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
		unsigned int month_date, unsigned year_date, bool buy_sell_trans,
		unsigned int number_shares, double trans_amount) {
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;
	if (buy_sell_trans == true) {
		trans_type = "Buy";
	}
	else {
		trans_type = "Sell";
	}
	shares = number_shares;
	amount = trans_amount;
	trans_id = 0;
	acb = 0.0;
	acb_per_share = 0.0;
	share_balance = 0;
	cgl = 0;
	p_next = nullptr;

	trans_id = assigned_trans_id;
	assigned_trans_id++;
}

// Destructor
// TASK
//

Transaction::~Transaction() {

}

// TASK
// Overloaded < operator.
//

bool Transaction::operator<(Transaction const &other) {

	if (this->year < other.get_year()) {
		return true;
	} else if (this->year == other.get_year()) {
		if (this->month < other.get_month()) {
			return true;
		} else if (this->month == other.get_month()) {
			if (this->day < other.get_day()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const {
	return symbol;
}
unsigned int Transaction::get_day() const {
	return day;
}
unsigned int Transaction::get_month() const {
	return month;
}
unsigned int Transaction::get_year() const {
	return year;
}
unsigned int Transaction::get_shares() const {
	return shares;
}
double Transaction::get_amount() const {
	return amount;
}
double Transaction::get_acb() const {
	return acb;
}
double Transaction::get_acb_per_share() const {
	return acb_per_share;
}
unsigned int Transaction::get_share_balance() const {
	return share_balance;
}
double Transaction::get_cgl() const {
	return cgl;
}
bool Transaction::get_trans_type() const {
	return (trans_type == "Buy") ? true : false;
}
unsigned int Transaction::get_trans_id() const {
	return trans_id;
}
Transaction* Transaction::get_next() {
	return p_next;
}

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb(double acb_value) {
	acb = acb_value;
}
void Transaction::set_acb_per_share(double acb_share_value) {
	acb_per_share = acb_share_value;
}
void Transaction::set_share_balance(unsigned int bal) {
	share_balance = bal;
}
void Transaction::set_cgl(double value) {
	cgl = value;
}
void Transaction::set_next(Transaction *p_new_next) {
	p_next = p_new_next;
}

// GIVEN
// Print the transaction.
//
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " " << std::setw(4)
			<< get_symbol() << " " << std::setw(4) << get_day() << " "
			<< std::setw(4) << get_month() << " " << std::setw(4) << get_year()
			<< " ";

	if (get_trans_type()) {
		std::cout << "  Buy  ";
	} else {
		std::cout << "  Sell ";
	}

	std::cout << std::setw(4) << get_shares() << " " << std::setw(10)
			<< get_amount() << " " << std::setw(10) << get_acb() << " "
			<< std::setw(4) << get_share_balance() << " " << std::setw(10)
			<< std::setprecision(3) << get_acb_per_share() << " "
			<< std::setw(10) << std::setprecision(3) << get_cgl() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
History::History() {
	p_head = nullptr;
}

// Destructor
// TASK
//
History::~History() {
	Transaction *current_node = p_head;
	unsigned int count = 0;

	while (current_node != nullptr) {
		count++;
		current_node = current_node->get_next();
	}

	current_node = p_head;
	for (int k = count; k > 0; k--) {
		current_node = p_head;
		for (int m = 0; m < k; m++) {
			current_node = current_node->get_next();
		}
		delete current_node;
		current_node = nullptr;
	}
	delete p_head;
	p_head = nullptr;
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
	ece150::open_file();
	while (ece150::next_trans_entry() == true) {
		Transaction *t = new Transaction(ece150::get_trans_symbol(),
				ece150::get_trans_day(), ece150::get_trans_month(),
				ece150::get_trans_year(), ece150::get_trans_type(),
				ece150::get_trans_shares(), ece150::get_trans_amount());
		insert(t);
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//

void History::insert(Transaction *p_new_trans) {

	if (p_head == nullptr) {
		p_head = p_new_trans;
	} else {
		Transaction *p_traverse = p_head;
		while (p_traverse->get_next() != nullptr) {
			p_traverse = p_traverse->get_next();
		}
		p_traverse->set_next(p_new_trans);
	}
}

// TASK
// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date() {
	Transaction *p_current = p_head;
	Transaction *p_head_2 = p_head;
	Transaction *next = p_head;
	Transaction *p_traverse = p_head_2;
	Transaction *prev = p_head_2;
	unsigned int count = 0;

	// count the number of transactions
	while (p_current != nullptr) {
		count++;
		p_current = p_current->get_next();
	}


	// set the first transaction as the first node in the new linked list
	p_current = p_head;
	p_head_2 = p_current;
	next = p_current->get_next();
	p_head = next;
	p_head_2->set_next(nullptr);


	// insertion sort: remove the first node of the original linked list and place it in the new linked list in ascending order

	// loops once for every node/transaction in the original linked list
	for (int k = 1; k < count; k++) {
		// resets to begin checking from the beginning of the new linked list
		p_current = p_head;
		next = p_current->get_next();
		p_head = next;
		p_traverse = p_head_2;
		// loops once for every node/transaction in the new linked list
		for (int j = 0; j < k; j++) {
			// special case: if there is only one node in the new linked list
			if (j == 0) {
				// if the current transaction is before the one in the new linked list, insert the transaction before
				if (*p_current < *p_traverse) {
					p_current->set_next(p_traverse);
					p_head_2 = p_current;
					break;
				}
				// if the date of the current transaction is after all others in the new linked list, insert the transaction at the end
				else {
					if (k == 1) {
						p_traverse->set_next(p_current);
						p_current->set_next(nullptr);
						p_head_2 = p_traverse;
						break;
					}
				}
			}
			// inserts the current transaction in the new linked list in so the new linked list is in ascending order
			else if (*p_current < *p_traverse) {
				prev->set_next(p_current);
				p_current->set_next(p_traverse);
				break;
			}
			// if the current transaction is after all the others in the linked list, insert the transaction at the end
			else {
				if (j == (k - 1)) {
					p_traverse->set_next(p_current);
					p_current->set_next(nullptr);
				}
			}
			// moves to next value in original linked list to be compared with new linked list
			prev = p_traverse;
			p_traverse = p_traverse->get_next();
		}
	}
	p_head = p_head_2;
}

// TASK
// update_acb_cgl(): Updates the ACB and CGL values.
//

void History::update_acb_cgl() {
	Transaction *t = p_head;
	double acb_value = 0.0;
	unsigned int acb_share_balance = 0;
	double acb_share_value = 0.0;
	double cgl_value = 0.0;

	while (t != nullptr) {
		if (t->get_trans_type() == true) {
			acb_value += t->get_amount();
			t->set_acb(acb_value);

			acb_share_balance += t->get_shares();
			t->set_share_balance(acb_share_balance);

			acb_share_value = acb_value/acb_share_balance;
			t->set_acb_per_share(acb_share_value);
		}
		else {
			acb_value -= t->get_shares()*acb_share_value;
			t->set_acb(acb_value);

			cgl_value = t->get_amount() - (t->get_shares() * acb_share_value);
			t->set_cgl(cgl_value);

			acb_share_balance -= t->get_shares();
			t->set_share_balance(acb_share_balance);

			acb_share_value = acb_value/acb_share_balance;
			t->set_acb_per_share(acb_share_value);
		}
		t = t->get_next();
	}
}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//

double History::compute_cgl(unsigned int year) {
	Transaction *t = p_head;
	double cgl_total = 0.0;
	while (t != nullptr) {
		if (t->get_year() == year) {
			cgl_total += t->get_cgl();
		}
		t = t->get_next();
	}
	return cgl_total;
}

// TASK
// print() Print the transaction history.
//

void History::print() {
	Transaction *n = p_head;
	if (p_head == nullptr) {
		return;
	}
	while (n != nullptr) {
		n->print();
		n = n->get_next();
	}
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction* History::get_p_head() {
	return p_head;
}

#endif

