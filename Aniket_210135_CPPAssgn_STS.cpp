#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<ctime>
#include<cmath>

#define SEC_IN_DAY 86400

using namespace std;

class date;
class planet;
class travel;
class traveller;
class ticket;
class astronaut;
class commander;
class passenger;
class space_travel;

//class declarations


map<int, ticket> ticket_hold;
map<int, passenger> passenger_hold;
map<int, astronaut> astronaut_hold;
map<int, commander> commander_hold;
map<int, space_travel> travel_hold;

//these maps hold the traveller, tickets and space_travel objects


void check_travel(set<int> &ids, map<int, ticket *> &m, ticket * curr, map<int, traveller *> &id_trav, astronaut *astro, commander *comm);
//declaration of function to check if a space travel class is to be formed


class date{
    //class to store date
    private:
        //internal representation using ctime tm struct
        struct tm date__;

    public:
        int dd;
        int mm;
        int yyyy;

        date(int date = -1, int month = -1, int year = -1);
        //constructor
        void update(int date = -1, int month = -1, int year = -1);
        //function to update the date
        int diff(date &date_in);
        //function to return difference between the provided date and object on which it is called
        //returns number of days between the dates, +ve output if provided date is after the date object
        time_t get_time();
        //get ctime time_t representation of date

        bool operator==(date &d1);
        //operator to check if two dates are equal
};

bool date::operator==(date &d1){
    return (dd == d1.dd && mm == d1.mm && yyyy == d1.yyyy);
}

date::date(int date, int month, int year){
    time_t today = time(NULL);
    struct tm local__ = *gmtime(&today);
    //default args yield present GMT date
    if(date == -1){
        date = local__.tm_mday;
    }
    if(month == -1){
        month = local__.tm_mon+1;
    }
    if(year == -1){
        year = local__.tm_year+1900;
    }

    dd = date;
    mm = month;
    yyyy = year;

    date__.tm_mday = dd;
    date__.tm_mon = mm - 1;
    date__.tm_year = yyyy - 1900;
    date__.tm_hour = 0;
    date__.tm_sec = 0;
    date__.tm_min = 0;
    date__.tm_gmtoff = 19800;
    date__.tm_isdst = 0;

    mktime(&date__);
}

void date::update(int date, int month, int year){
    time_t today = time(NULL);
    struct tm local__ = *gmtime(&today);
    if(date == -1){
        date = local__.tm_mday;
    }
    if(month == -1){
        month = local__.tm_mon+1;
    }
    if(year == -1){
        year = local__.tm_year+1900;
    }

    dd = date;
    mm = month;
    yyyy = year;

    date__.tm_mday = dd;
    date__.tm_mon = mm - 1;
    date__.tm_year = yyyy - 1900;
    date__.tm_hour = 0;
    date__.tm_sec = 0;
    date__.tm_min = 0;
}

time_t date::get_time(){
    return mktime(&date__);
}

int date::diff(date &date_in){
    //return days between dates
    time_t temp = mktime(&date__);
    double diff_sec = difftime(date_in.get_time(), temp);
    return diff_sec/SEC_IN_DAY;
}




class planet{
    //class to define planet
    public:
        string name;
        int x_coord;
        int y_coord;
        int z_coord;

        planet(int x = 0, int y = 0, int z = 0, string s = "default_name");
        //constructor
        vector<int> get_coordinates();
        //getter for coordinates
        double distance(planet *pl1 = NULL);
        //return distance between planet pointed to by pl1 and object on which it is called
};

planet::planet(int x, int y, int z, string s){
    x_coord = x;
    y_coord = y;
    z_coord = z;
    name = s;
}

vector<int> planet::get_coordinates(){
    vector<int> temp;
    temp.push_back(x_coord);
    temp.push_back(y_coord);
    temp.push_back(z_coord);
    return temp;
}

double planet::distance(planet *pl1){
    if(pl1 != NULL){
        int x_diff = (x_coord - pl1->x_coord)*(x_coord - pl1->x_coord);
        int y_diff = (y_coord - pl1->y_coord)*(y_coord - pl1->y_coord);
        int z_diff = (z_coord - pl1->z_coord)*(z_coord - pl1->z_coord);
        return sqrt(x_diff + y_diff + z_diff);
    }
    else{
        cerr<<"INVALID PLANET INPUT FOR DISTANCE MEASUREMENT\n";
        return -1;
    }
}




class ticket{
    //class for ticket
    private:
        double price;
    public:
        planet *source;
        planet *destination;
        date travel_date;
        string traveller_name;
        int traveller_ID;
        date valid_till;
        int ticket_ID;

        ticket();
        //constructor without initialisation
        ticket(set<int> &s, map<int, ticket *> &m, date *date_in = NULL, string *name_in = NULL, int ID_in = -1, planet *source_in = NULL, planet *destination_in = NULL);
        //constructor which can initialise
        bool update_ticket(date *date_in = NULL, planet *destination_in = NULL);
        //function to update the ticket on which it is called
        void set_price();
        double get_price();
        //setter and getter for price
};

ticket::ticket(){

}

ticket::ticket(set<int> &s, map<int, ticket *> &m, date *date_in, string *name_in, int ID_in, planet *source_in, planet *destination_in){
    ticket_ID = (s.empty()==true?1:(*(s.rbegin())+1));
    s.insert(ticket_ID);
    //system assigned ID
    
    travel_date = *date_in;
    traveller_name = *name_in;
    source = source_in;
    traveller_ID = ID_in;
    destination = destination_in;
    time_t today = time(NULL);
    date temp(gmtime(&today)->tm_mday, gmtime(&today)->tm_mon +1, gmtime(&today)->tm_year + 1910);
    
    valid_till = temp;
    //valid upto 10 yrs from booking date
    set_price();
}

bool ticket::update_ticket(date *date_in, planet *destination_in){
    if(date_in->diff(valid_till) >= 0) {
        travel_date = *date_in;
        destination = destination_in;
        set_price();
        return true;
    }
    return false;
    //return true and update ticket if date is valid else false
}

void ticket::set_price(){
    date today(-1,-1,-1);
    double price_const = source->distance(destination);

    price = (100000*price_const)/(1 + abs(today.diff(travel_date)));
    //price depends on both distance and how near the travel date is
}

double ticket::get_price(){
    return price;
}





class traveller{
    //base class for traveller
    public:
        string name;
        int ID;
        vector<ticket *> list_of_tickets;
        vector<space_travel *> list_of_travels;

    public:
        traveller();
        traveller(set<int> &s, string *name_in = NULL, int ID_in = -1);
        string get_name();
        int get_ID();
        bool verify_ID(set<int> &s, int ID_in = -1);
        void book_ticket(set<int> &s, map<int, ticket *> &m, date *date_in, planet *source_in, planet *destination_in, map<int, traveller *> &id_trav, vector<astronaut *> &a, vector<commander *> &c);
        void cancel_ticket(std::vector<ticket *>::iterator cancelled, set<int> &s, map<int, ticket *> &m);
};

traveller::traveller(){

}

traveller::traveller(set<int> &s, string *name_in, int ID_in){
    //constructor
    name = *name_in;
    if(ID_in == -1 || !verify_ID(s, ID_in)){
        if(s.empty()){
            s.insert(1);
            ID = 1;
            cerr<<"You entered an invalid/disallowed ID. The system assigns you an ID: "<<ID<<"\n";
        }
        else{
            int temp_ID = *s.rbegin() + 1;
            s.insert(temp_ID);
            ID = temp_ID;
            cerr<<"You entered an invalid/disallowed ID. The system assigns you an ID: "<<ID<<"\n";
        }
    }
    else{
        ID = ID_in;
        s.insert(ID_in);
    }
}

string traveller::get_name(){
    return name;
}

int traveller::get_ID(){
    return ID;
}

bool traveller::verify_ID(set<int> &s, int ID_in){
    if(ID_in > 0 && s.find(ID_in) == s.end()){
        return true;
    }
    else return false;
}

void traveller::book_ticket(set<int> &s, map<int, ticket *> &m, date *date_in, planet *source_in, planet *destination_in, map<int, traveller *> &id_trav, vector<astronaut *> &a, vector<commander *> &c){
    if(date_in != NULL && source_in != NULL && destination_in != NULL){
        ticket temp(s, m, date_in, &name, ID, source_in, destination_in);
        ticket_hold[temp.ticket_ID] = temp;
        //create new ticket in the holding container
        m[temp.ticket_ID] = &(ticket_hold[temp.ticket_ID]);
        ticket *new_ticket = &(ticket_hold[temp.ticket_ID]);
        list_of_tickets.push_back(new_ticket);
        check_travel(s, m, new_ticket, id_trav, a[0], c[0]);
        //check if a travel can be created ie 2 or more guys with same date, source and destination
    }
}

void traveller::cancel_ticket(std::vector<ticket *>::iterator cancelled, set<int> &s, map<int, ticket *> &m){
    ticket *temp = *cancelled;
    s.erase(temp->ticket_ID);
    m[temp->ticket_ID] = NULL;
    list_of_tickets.erase(cancelled);
}





class astronaut: public traveller{
    //derived class from traveleler 
    public:
        int experience;

    astronaut();
    astronaut(set<int> &s, int exp_in = 0, string *name_in = NULL, int ID_in = -1);
};

astronaut::astronaut(){

}

astronaut::astronaut(set<int> &s, int exp_in, string *name_in, int ID_in): traveller(s, name_in, ID_in){
    experience = exp_in;
}




class passenger: public traveller{
    //derived from traveller
    public:
        passenger();
        passenger(set<int> &s, string *name_in = NULL, int ID_in = -1);
};

passenger::passenger(){

}

passenger::passenger(set<int> &s, string *name_in, int ID_in): traveller(s, name_in, ID_in){

}




class commander: public traveller{
    //class for commander
    private:
        string authority;

    public:
        int experience;
        string license_ID;
        static vector<commander *> list_of_commanders;

        commander();
        commander(set<int> &s, string* authority_in = NULL, int exp = 0, string* license_in = NULL, string *name_in = NULL, int ID_in = -1);
        void set_authority(string *s = NULL);
        string get_authority();
};

commander::commander(){

}

commander::commander(set<int> &s, string *authority_in, int exp, string *license_in, string *name_in, int ID_in): traveller(s, name_in, ID_in)
{
    authority = *authority_in;
    experience = exp;
    license_ID = *license_in;
}

void commander::set_authority(string *s){
    authority = *s;
}

string commander::get_authority(){
    return authority;
}




class space_travel{
    //class for space travel
    public:
        vector<traveller *> list_of_travellers;
        astronaut *astro;
        commander *command;
        int travel_ID;
        static int travel_ID_generator;
        date travel_date;
        planet *source;
        planet *destination;
        static vector<space_travel *> all_travels;

        space_travel();
        space_travel(vector<traveller *> &list, astronaut *astr, commander *comm, date &date_travel_in, planet *source_in, planet *destination_in);
        void add_traveller(traveller *pass);
        void list_travellers();
        void set_astronaut(astronaut *astr);
        void set_commander(commander *comm);
        void remove_traveller(traveller *pass);
        int no_of_trav();
};

space_travel::space_travel(){

}

space_travel::space_travel(vector<traveller *> &list, astronaut *astr, commander *comm, date &date_travel_in, planet *source_in, planet *destination_in){
    //constructor
    list_of_travellers = list;
    travel_date = date_travel_in;
    source = source_in;
    destination = destination_in;
    set_astronaut(astr);
    set_commander(comm);
    travel_ID = travel_ID_generator;
    travel_ID_generator++;
};

int space_travel::no_of_trav(){
    //returns number of travellers
    return list_of_travellers.size();
}

void space_travel::add_traveller(traveller *pass){
    list_of_travellers.push_back(pass);
}

void space_travel::list_travellers(){
    //lists the traveller
    for(auto x: list_of_travellers){
        cout<<x->get_name()<<"\n";
    }
    cout<<"Astronaut: "<<astro->get_name()<<"\n";
    cout<<"Commander: "<<command->get_name()<<"\n";
}

void space_travel::set_astronaut(astronaut *astr){
    astro = astr;
}

void space_travel::set_commander(commander *comm){
    command = comm;
}

void space_travel::remove_traveller(traveller *pass){
    //removes traveller
    bool flag = false;
    auto x = list_of_travellers.begin();
    while(!flag || x!= list_of_travellers.end()){
        if(*x == pass){
            list_of_travellers.erase(x);

            for(auto y = pass->list_of_travels.begin(); y != pass->list_of_travels.end(); y++){
                if(*y == this){
                    pass->list_of_travels.erase(y);
                    break;
                }
            }
            break;
        }
        x++;
    }
}




void check_travel(set<int> &ids, map<int, ticket *> &m, ticket * curr, map<int, traveller *> &id_trav, astronaut *astro, commander *comm){

//checks if travel class can e created

    vector<traveller *> list;
    int cnt = 0;
    for(auto x: ids){
        if(m[x] != NULL){
            if(m[x]->source == curr->source && m[x]->destination == curr->destination && m[x]->travel_date == curr->travel_date){
                cnt++;
                list.push_back(id_trav[m[x]->traveller_ID]);
            }
        }
    }
    if(cnt == 2) {
        space_travel temp(list, astro, comm, curr->travel_date, curr->source, curr->destination);
        travel_hold[temp.travel_ID] = temp;
        space_travel *new_travel = &(travel_hold[temp.travel_ID]) ;
        space_travel::all_travels.push_back(new_travel);
        new_travel->astro->list_of_travels.push_back(new_travel);
        new_travel->command->list_of_travels.push_back(new_travel);
        for(auto x : new_travel->list_of_travellers){
            x->list_of_travels.push_back(new_travel);
        }
    }
    else if(cnt > 2){
        for(auto x: space_travel::all_travels){
            if(x->source == curr->source && x->destination == curr->destination && curr->travel_date == x->travel_date){
                x->add_traveller(id_trav[curr->traveller_ID]);
            }
        }
    }
}

void create_universe(vector<passenger *> &p, vector<astronaut *> &a, vector<commander *> &c, set<int> &s, map<int, traveller *> &m){
    //create universe with travellers
    string trav_name = "Pas";
    for(int i = 0; i<9; i++){
        trav_name.append(string(1, '1' + i));
        passenger temp(s, &trav_name, i+1);
        passenger_hold[temp.ID] = temp;
        auto new_pass = &(passenger_hold[temp.ID]);
        p.push_back(new_pass);
        m[p.back()->get_ID()] = p.back();
        trav_name.pop_back();
    }
    
    trav_name.append("10");
    passenger temp(s, &trav_name, 10);
    passenger_hold[temp.ID] = (temp);
    auto new_pass = &(passenger_hold[temp.ID]);
    p.push_back(new_pass);
    
    m[p.back()->get_ID()] = p.back();
    trav_name.pop_back();
    trav_name.pop_back();
    trav_name.append("11");
    passenger temp2(s, &trav_name, 11);
    passenger_hold[temp2.ID] = temp2;
    new_pass = &(passenger_hold[temp2.ID]);
    p.push_back(new_pass);
    m[p.back()->get_ID()] = p.back();

    trav_name = "Astro1";
    astronaut temp3(s, 10, &trav_name, 12);
    astronaut_hold[temp3.ID] = temp3;
    auto new_astro = &(astronaut_hold[temp3.ID]);
    a.push_back(new_astro);
    m[a.back()->get_ID()] = a.back();

    trav_name = "Astro2";
    astronaut temp4(s, 10, &trav_name, 13);
    astronaut_hold[temp4.ID] = temp4;
    new_astro = &(astronaut_hold[temp4.ID]);
    a.push_back(new_astro);
    m[a.back()->get_ID()] = a.back();


    trav_name = "Comm1";
    string authority = "Authority1";
    string license = "EARTH01";
    commander temp5(s, &authority, 20, &license, &trav_name, 14);
    commander_hold[temp5.ID] = temp5;
    c.push_back(&(commander_hold[temp5.ID]));
    commander::list_of_commanders.push_back(&(commander_hold[temp5.ID]));
    m[c.back()->get_ID()] = c.back();

    trav_name = "Comm2";
    authority = "Authority02";
    license = "EARTH02";
    commander temp6(s, &authority, 25, &license, &trav_name, 15);
    commander_hold[temp6.ID] = temp6;
    c.push_back(&(commander_hold[temp6.ID]));
    m[c.back()->get_ID()] = c.back();

}

template <typename T>
void change_user(vector<T *> &a, typename std::vector<T *>::iterator &curr_a){
    //template function for changing current user
    bool case_flag = false;
    while(!case_flag){
        case_flag = true;
        int i = 1;
        cout<<"Enter the number of the user you want to change to.\n";
        for(auto x: a){
            cout<<i<<" "<<x->get_name()<<"\n";
            i++;
        }
        cin>>i;
        if(i<1 || i>a.size()){
            cout<<"Invalid input . Please enter 1 to do the previous operation again, or enter 0 to go back to previous menu\n";
            int opcode = 0;
            cin>>opcode;
            if(opcode) case_flag = false;
        }
        else{
            curr_a = a.begin() + i - 1;
            cout<<"Changed to "<<(*curr_a)->get_name()<<"\n";
        }
    }
}

template <typename T>
void update_ticket_input(typename std::vector<T *>::iterator &curr_a, map<string, planet *> &name_to_planet, set<int> &s/*ticket ids*/, map<int, ticket *> &m, map<int, traveller *> &ID_trav, vector<astronaut *> &a, vector<commander *> &c){
    //handler forinput for update
    bool case_flag = false;
    while(!case_flag){
        cout<<"Please enter the number of the ticket you want to update, enter 0 to exit: \n";
        int  i = 0;
        for(auto x: (*curr_a)->list_of_tickets){
            cout<<i+1<<" "<<x->source->name<<" "<<x->destination->name;
            cout<<" "<<x->travel_date.dd<<"/"<<x->travel_date.mm<<"/"<<x->travel_date.yyyy<<"\n";
            i++;
        }
        cin>>i;
        if(i<1 || i>(*curr_a)->list_of_tickets.size()){
            int opcode;
            cout<<"Invalid input. Please enter 1 to rechoose the ticket, else press 0 to exit\n";
            cin>>opcode;
            if(!opcode) case_flag = true;
        }
        else{
            auto curr_destination = (*curr_a)->list_of_tickets[i-1]->destination;  
            auto old_dest = curr_destination; //pointer to planet
            auto curr_travel_date = (*curr_a)->list_of_tickets[i-1]->travel_date;  
            auto old_date = curr_travel_date; //date
            cout<<"Please enter the name of the new destination planet (enter -1 to skip changing destination): ";
            string name_destination;
            cin>>name_destination;
        
            auto x = name_to_planet.find(name_destination);
            if(x == name_to_planet.end()){
                cout<<"Planet not found, destination won't be changed\n";
            }
            else curr_destination = x->second;
            int dd, mm, yyyy;
            cout<<"Please enter the name of the new date of travel (enter -1 to skip changing the field): \nDD:";
            cin>>dd;
            if(dd == -1){
                cout<<"DD unchanged.\n";
                dd = curr_travel_date.dd;
            }
            cout<<"MM:";
            cin>>mm;
            if(mm == -1){
                cout<<"MM unchanged.\n";
                mm = curr_travel_date.mm;
            }
            cout<<"YYYY:";
            cin>>yyyy;
            if(yyyy == -1){
                cout<<"YYYY unchanged.\n";
                yyyy = curr_travel_date.yyyy;
            }

            curr_travel_date.update(dd, mm, yyyy);
            
            case_flag = (*curr_a)->list_of_tickets[i-1]->update_ticket(&curr_travel_date, curr_destination);

            if(!case_flag){
                cout<<"Invalid date - your ticket will be invalid then. Press 1 to rechoose the date, or 0 to exit\n";
                int opcode;
                cin>>opcode;
                if(!opcode) case_flag = true;
            }
            else{

                for(auto zi = space_travel::all_travels.begin(); zi != space_travel::all_travels.end(); zi++){
                    auto z = *zi;
                    if(z == NULL) continue;
                    if(z->destination == old_dest && z->source == (*curr_a)->list_of_tickets[i-1]->source && z->travel_date == old_date){
                        z->remove_traveller(*curr_a);
                        auto y = zi;
                        if(z->no_of_trav() < 2){
                            for(auto x = z->astro->list_of_travels.begin(); x != z->astro->list_of_travels.end(); x++){
                                if(*x == z){
                                    z->astro->list_of_travels.erase(x);
                                    break;
                                }
                            }
                            for(auto x = z->command->list_of_travels.begin(); x != z->command->list_of_travels.end(); x++){
                                if(*x == z){
                                    z->command->list_of_travels.erase(x);
                                    break;
                                }
                            }
                            
                            for(auto y: z->list_of_travellers){
                                for(auto x = y->list_of_travels.begin(); x != y->list_of_travels.end(); x++){
                                    if(*x == z){
                                        z->astro->list_of_travels.erase(x);
                                        break;
                                    }
                                }
                            }
                            for(auto x = space_travel::all_travels.begin(); x != space_travel::all_travels.end(); x++){
                                if(*x == z){
                                    space_travel::all_travels.erase(x);
                                    break;
                                }
                            }
                            
                            space_travel::all_travels.erase(zi);
                        } 
                        break;
                        
                    }
                }
                check_travel(s, m, (*curr_a)->list_of_tickets[i-1], ID_trav, a[0], c[0]);
                cout<<"Ticket updated.\n";
            }
        }
        
    }
}


template <typename T>
void book_ticket_input(typename std::vector<T *>::iterator &curr_a, map<string, planet *> &name_to_planet, set<int> &ticket_IDs, map<int, ticket *> &ID_to_ticket, map<int, traveller *> &id_trav, vector<astronaut *> &a, vector<commander *> &c){
    //handler for input of booking ticket
    bool case_flag = false;
    int temp_flag = 0;
    planet *source;
    planet *dest;
    while(!case_flag){
        bool except_flag = false;
        try{
            cout<<"Please enter the source planet:\n";
            string name_planet;
            cin>>name_planet;
            auto x = name_to_planet.find(name_planet);
            source = (*x).second;
            if(x == name_to_planet.end()){
                throw "Planet not found. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            cout<<"Please enter the name of the destination planet: ";
            cin>>name_planet;
            auto y = name_to_planet.find(name_planet);
            dest = (*y).second;
            if(y == name_to_planet.end()){
                throw "Planet not found. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            int dd, mm, yyyy;
            cout<<"Please enter the date of travel: \nDD:";
            cin>>dd;
            if(dd < 1 && dd > 31){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            cout<<"MM:";
            cin>>mm;
            if(mm < 1 && mm > 12){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            cout<<"YYYY:";
            cin>>yyyy;
            time_t now = time(NULL);
            struct tm gmt_now = *gmtime(&now);
            if(yyyy < gmt_now.tm_year + 1900){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            date travel_date(dd, mm, yyyy);
            
            date temp(gmt_now.tm_mday, gmt_now.tm_mon +1, gmt_now.tm_year + 1910);
            
            int differ = temp.diff(travel_date);
            if(differ > 0){
                throw "This ticket is only valid for 10 years from date of booking, cannot book this date. Press 1 to re-choose, 0 to return to previous menu\n";
            }

            (*curr_a)->book_ticket(ticket_IDs, ID_to_ticket, &travel_date, (*x).second, (*y).second, id_trav, a, c);
            
        }
        catch (char const *excpt){
            except_flag = true;
            cout<<excpt;
            int opcode;
            cin>>opcode;
            if(!opcode){
                case_flag = true;
            }
        }
        if(!except_flag){
            // int temp = 0;
            cout<<"Do you want a return ticket? 1 for yes, 0 for no:\n";
            cin>>temp_flag;
            case_flag = true;
        }
    }
    
    bool return_flag = false;
    while(!return_flag && temp_flag){
        bool except_flag = false;
        try{
            auto x = dest;
            auto y = source;
            int dd, mm, yyyy;
            cout<<"Please enter the date of travel: \nDD:";
            cin>>dd;
            if(dd < 1 && dd > 31){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            cout<<"MM:";
            cin>>mm;
            if(mm < 1 && mm > 12){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            cout<<"YYYY:";
            cin>>yyyy;
            time_t now = time(NULL);
            struct tm gmt_now = *gmtime(&now);
            if(yyyy < gmt_now.tm_year + 1900){
                throw "Incorrect date format. Press 1 to re-choose, 0 to return to previous menu\n";
            }
            date travel_date(dd, mm, yyyy);
            
            date temp(gmt_now.tm_mday, gmt_now.tm_mon +1, gmt_now.tm_year + 1910);
            
            int differ = temp.diff(travel_date);
            if(differ > 0){
                throw "This ticket is only valid for 10 years from date of booking, cannot book this date. Press 1 to re-choose, 0 to return to previous menu\n";
            }

            (*curr_a)->book_ticket(ticket_IDs, ID_to_ticket, &travel_date, x, y, id_trav, a, c);
            
        }
        catch (char const *excpt){
            except_flag = true;
            cout<<excpt;
            int opcode;
            cin>>opcode;
            if(!opcode){
                return_flag = true;
            }
        }
        if(!except_flag){
            return_flag = true;
        }
    }
}

template<typename T>
void cancel_ticket_input(typename std::vector<T *>::iterator &curr_a, set<int> &ticket_IDs, map<int, ticket *> &ID_to_ticket, map<int, traveller *> &id_to_trav){
    //handler for cancel input
    bool case_flag = false;
    while(!case_flag){
        cout<<"Please choose the number of the ticket you want to cancel:\n";
        int  i = 0;
        for(auto x: (*curr_a)->list_of_tickets){
            cout<<i+1<<" "<<x->source->name<<" "<<x->destination->name;
            cout<<" "<<x->travel_date.dd<<"/"<<x->travel_date.mm<<"/"<<x->travel_date.yyyy<<"\n";
            i++;
        }
        cin>>i;
        if(i<1 || i>(*curr_a)->list_of_tickets.size()){
            int opcode;
            cout<<"Invalid input. Please enter 1 to rechoose the ticket, else press 0 to exit\n";
            cin>>opcode;
            if(!opcode) case_flag = true;
        }
        else{
            auto cancelled = (*curr_a)->list_of_tickets.begin() + i - 1;
            for(auto zi = space_travel::all_travels.begin(); zi != space_travel::all_travels.end(); zi++){
                auto z = *zi;
                if(z->destination == (*cancelled)->destination && z->source == (*cancelled)->source && z->travel_date == (*cancelled)->travel_date){
                    z->remove_traveller(id_to_trav[(*cancelled)->traveller_ID]);
                    if(z->no_of_trav() < 2){

                        
                        for(auto x = z->astro->list_of_travels.begin(); x != z->astro->list_of_travels.end(); x++){
                            if(*x == z){
                                z->astro->list_of_travels.erase(x);
                                break;
                            }
                        }
                        for(auto x = z->command->list_of_travels.begin(); x != z->command->list_of_travels.end(); x++){
                            if(*x == z){
                                z->command->list_of_travels.erase(x);
                                break;
                            }
                        }
                        for(auto y: z->list_of_travellers){
                            for(auto x = y->list_of_travels.begin(); x != y->list_of_travels.end(); x++){
                                if(*x == z){
                                    z->astro->list_of_travels.erase(x);
                                    break;
                                }
                            }
                        }
                        for(auto x = space_travel::all_travels.begin(); x != space_travel::all_travels.end(); x++){
                            if(*x == z){
                                space_travel::all_travels.erase(x);
                                break;
                            }
                        }
                        space_travel::all_travels.erase(zi);


                    }
                    break;
                }
            }
            (*curr_a)->cancel_ticket(cancelled, ticket_IDs, ID_to_ticket);
            case_flag = true;
        }
    }
}

template<typename T>
void handle_print(typename std::vector<T *>::iterator &curr_a){
    bool case_flag = false;
    while(!case_flag){
        cout<<"Choose the ticket serial no. you want to print, press 0 to exit:\n";
        int  i = 0;
        for(auto x: (*curr_a)->list_of_tickets){
            cout<<i+1<<" "<<x->source->name<<" "<<x->destination->name;
            cout<<" "<<x->travel_date.dd<<"/"<<x->travel_date.mm<<"/"<<x->travel_date.yyyy<<"\n";
            i++;
        }
        cin>>i;
        if(i<1 || i>(*curr_a)->list_of_tickets.size()){
            int opcode;
            cout<<"Invalid input. Please enter 1 to rechoose the ticket, else press 0 to exit\n";
            cin>>opcode;
            if(!opcode) case_flag = true;
        }
        else{
            auto printed = (*curr_a)->list_of_tickets.begin() + i - 1;
            cout<<"Name of Traveller: "<<(*printed)->traveller_name<<"\n";
            cout<<"Traveller ID: "<<(*printed)->traveller_ID<<"\n";
            cout<<"Ticket ID: "<<(*printed)->ticket_ID<<"\n";
            cout<<"Source Planet: "<<(*printed)->source->name<<"\n";
            cout<<"Destination Planet: "<<(*printed)->destination->name<<"\n";
            cout<<"Date of travel: "<<(*printed)->travel_date.dd<<"/"<<(*printed)->travel_date.mm<<"/"<<(*printed)->travel_date.yyyy<<"\n";
            cout<<"Valid Till: "<<(*printed)->valid_till.dd<<"/"<<(*printed)->valid_till.mm<<"/"<<(*printed)->valid_till.yyyy<<"\n";
            cout<<"Price: "<<(*printed)->get_price()<<"\n";
            case_flag = true;
        }
    }
}




vector<commander *> commander::list_of_commanders;


vector<space_travel *> space_travel::all_travels;
int space_travel::travel_ID_generator = 0;




int main(){
    set<int> ids;   //set of traveller_IDs
    map<int, traveller *> ID_to_traveller; //map from traveller_ID to traeller pointer
    set<int> ticket_IDs;
    map<int, ticket *> ID_to_ticket; //map from tickt ID to tickets pointer

    vector<passenger *> p;
    vector<astronaut *> a;
    vector<commander *> c;

    create_universe(p, a, c, ids, ID_to_traveller);
    
    map<string, planet *> name_to_planet;
    planet earth(0,0,0, "Earth");
    name_to_planet["Earth"] = &earth;
    planet mars(10, 10, 10, "Mars");
    name_to_planet["Mars"] = &mars;
    planet jupiter(20, 100, 431, "Jupiter");
    name_to_planet["Jupiter"] = &jupiter;

    int s;
    auto curr_p = p.begin();
    auto curr_a = a.begin();
    auto curr_c = c.begin();
    int type = 1;

    bool exit_flag = false;
    



    while(!exit_flag){
        cout<<"Please enter your role:\n1 for Passenger\n2 for Astronaut\n3 for Commander\n\nPress 0 to exit\n";
        int role = 4;
        cin>>role;
        switch(role){
            case 0:{
                exit_flag = true;
                break;
            }
            case 1:{
                bool role_flag = false;
                while(!role_flag){
                    int s;
                    cout<<"\n\nWhat do you want to do today?\n1: Choose User\n2: Ticket Book/Modify/Cancel\n3: Print Ticket Details\n4: Exit\n";
                    cin>>s;
                    switch(s){
                        case 1: {
                            change_user<passenger>(p, curr_p);
                            //change user
                            break;
                        }
                        case 2: {
                            //ticket ops

                            bool func_flag = false;
                            while(!func_flag){
                                // func_flag = true;
                                cout<<"\nEnter 1 for Booking New Ticket,\n2 for Modifying your tickets,\n3 for Cancelling your tickets,\n4 to go back to previous menu\n";
                                int op = 0;
                                cin>>op;
                                switch(op){
                                    case 1:{
                                        book_ticket_input<passenger> (curr_p, name_to_planet, ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 2:{
                                        update_ticket_input<passenger> (curr_p, name_to_planet, ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 3:{
                                        cancel_ticket_input<passenger> (curr_p, ticket_IDs, ID_to_ticket, ID_to_traveller);
                                        break;
                                    }
                                    case 4:{
                                        func_flag = true;
                                        break;
                                    }
                                    default:{
                                        cout<<"Invalid input. Please enter 1 to do the previous operation again, or enter 0 to go back to main menu\n";
                                        int opcode = 0;
                                        cin>>opcode;
                                        if(opcode) func_flag = false;
                                    }
                                }
                            }
                            break;
                        }
                        case 3: {   
                            //print
                            handle_print<passenger>(curr_p);
                            break;
                        }
                        case 4:{
                            //exit
                            role_flag = true;
                        }
                    }
                }
                break;
            }
            case 2:{
                bool role_flag = false;
                while(!role_flag){
                    int s;
                    cout<<"\n\nWhat do you want to do today?\n1: Choose User\n2: Ticket Book/Modify/Cancel\n3: Print Ticket Details\n4: Exit\n";
                    cin>>s;
                    switch(s){
                        case 1: {
                            change_user<astronaut>(a, curr_a);
                            //change user
                            break;
                        }
                        case 2: {
                            //ticket ops

                            bool func_flag = false;
                            while(!func_flag){
                                // func_flag = true;
                                cout<<"\nEnter 1 for Booking New Ticket,\n2 for Modifying your tickets,\n3 for Cancelling your tickets,\n4 to go back to previous menu\n";
                                int op = 0;
                                cin>>op;
                                switch(op){
                                    case 1:{
                                        book_ticket_input<astronaut> (curr_a, name_to_planet,ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 2:{
                                        update_ticket_input<astronaut> (curr_a, name_to_planet,ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 3:{
                                        cancel_ticket_input<astronaut> (curr_a, ticket_IDs, ID_to_ticket, ID_to_traveller);
                                        break;
                                    }
                                    case 4:{
                                        func_flag = true;
                                        break;
                                    }
                                    default:{
                                        cout<<"Invalid input. Please enter 1 to do the previous operation again, or enter 0 to go back to main menu\n";
                                        int opcode = 0;
                                        cin>>opcode;
                                        if(opcode) func_flag = false;
                                    }
                                }
                            }
                            break;
                        }
                        case 3: {   
                            //print
                            handle_print<astronaut>(curr_a);
                            break;
                        }
                        case 4:{
                            //exit
                            role_flag = true;
                        }
                    }
                }
                break;
            }
            case 3:{
                bool role_flag = false;
                while(!role_flag){
                    int s;
                    cout<<"\n\nWhat do you want to do today?\n1: Choose User\n2: Ticket Book/Modify/Cancel\n3: Print Ticket Details\n4: Exit\n5: View Travels\n";
                    cin>>s;
                    switch(s){
                        case 1: {
                            change_user<commander>(c, curr_c);
                            //change user
                            break;
                        }
                        case 2: {
                            //ticket ops

                            bool func_flag = false;
                            while(!func_flag){
                                // func_flag = true;
                                cout<<"\nEnter 1 for Booking New Ticket,\n2 for Modifying your tickets,\n3 for Cancelling your tickets,\n4 to go back to previous menu\n";
                                int op = 0;
                                cin>>op;
                                switch(op){
                                    case 1:{
                                        book_ticket_input<commander> (curr_c, name_to_planet, ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 2:{
                                        update_ticket_input<commander> (curr_c, name_to_planet, ticket_IDs, ID_to_ticket, ID_to_traveller, a, c);
                                        break;
                                    }
                                    case 3:{
                                        cancel_ticket_input<commander> (curr_c, ticket_IDs, ID_to_ticket, ID_to_traveller);
                                        break;
                                    }
                                    case 4:{
                                        func_flag = true;
                                        break;
                                    }
                                    default:{
                                        cout<<"Invalid input. Please enter 1 to do the previous operation again, or enter 0 to go back to main menu\n";
                                        int opcode = 0;
                                        cin>>opcode;
                                        if(opcode) func_flag = false;
                                    }
                                }
                            }
                            break;
                        }
                        case 3: {   
                            //print
                            handle_print<commander>(curr_c);
                            break;
                        }
                        case 4:{
                            //exit
                            role_flag = true;
                        }
                        case 5:{
                            for(auto x: (*curr_c)->list_of_travels){
                                cout<<"Source Planet: "<<x->source->name<<"\n";
                                cout<<"Destination Planet: "<<x->destination->name<<"\n";
                                cout<<"Date of travel: "<<x->travel_date.dd<<"/"<<x->travel_date.mm<<"/"<<x->travel_date.yyyy<<"\n";
                                x->list_travellers();
                            }
                        }
                    }
                }
                break;
            }
            default:{
                cout<<"Please enter a valid role. Press 1 to re-enter role, 0 to exit program";
                int opcode;
                cin>>opcode;
                if(!opcode){
                    exit_flag = true;
                }
            }
        }
    }
    return 0;
}

