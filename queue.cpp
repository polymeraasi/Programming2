#include "queue.hh"
#include <iostream>

// Implement the member functions of Queue here

//rakentaja
Queue::Queue(unsigned int cycle): cycle_(cycle)
{
}
//purkaja
Queue::~Queue()
{
    Vehicle* current = first_;
    Vehicle* next = nullptr;

    while ( current != nullptr)
    {
        next = current->next;
        delete current;
        current = next;
    }
}

//lisätään autoja jonon perään
void Queue::enqueue(const string &reg)
{
    if ( is_green_)
    {
        cout << "GREEN: The vehicle " << reg << " need not stop to wait" << endl;
        return;
    }

    Vehicle* new_car = new Vehicle{reg, nullptr};

    // jos lista on tyhjä (lisätään ensimmäinen auto):
    if (is_empty())
    {
        first_ = new_car;
        last_ = new_car;

    } else {
        last_->next = new_car;
        last_ = new_car;
    }
}

void Queue::switch_light()
{
    //to switch the light:
    is_green_ = ! is_green_;

    if (is_empty())
    {
        print();
        return;
    }

    if (is_green_)
    {
        string number = "";
        cout << "GREEN: Vehicle(s) ";
        for ( unsigned int num = 0; num < cycle_; ++num)
        {
            if (is_empty())
            {
                break;
            }

            dequeue(number);
            cout << number << " ";
        }

        cout << "can go on" << endl;
        is_green_ = ! is_green_;
    }
}

void Queue::reset_cycle(unsigned int cycle)
{
    cycle_ = cycle;
}

void Queue::print()
{
    if ( is_green_)
    {
        cout << "GREEN: ";

    } else {
        cout << "RED: ";
    }

    if (is_empty())
    {
        cout << "No vehicles waiting in traffic lights" << endl;
        return;

    } else {
    cout << "Vehicle(s) ";
    Vehicle* current = first_;
    while (current != nullptr)
    {
        cout << current->reg_num << " ";
        current = current->next;
    }

    cout << "waiting in traffic lights" << endl;
    }
}

void Queue::dequeue(string& number)
{
    if ( !is_empty())
    {
        number = first_->reg_num;
        if (first_ == last_)
        {
            last_ = nullptr;
        }

        Vehicle* current = first_;
        first_ = first_->next;
        delete current;
    }
}

bool Queue::is_empty()
{
    return first_ == nullptr;
}


