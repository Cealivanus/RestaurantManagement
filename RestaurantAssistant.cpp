#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip> // 用于 setw

using namespace std;

class Dish {
private:
    int id;
    string name;
    double price;

public:
    Dish(int id, const string& name, double price) : id(id), name(name), price(price) {}

    int getID() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
};

// 定义顾客类
class Customer {
private:
    int tableNumber;
    int queueNumber;
    vector<Dish> dishes;

public:
    Customer(int table, int queue, int dishCount, const vector<int>& dishIds, const vector<Dish>& allDishes) :tableNumber(table), queueNumber(queue) 
    {
        for (int i = 0; i < dishCount; ++i) 
        {
            for (const auto& dish : allDishes) 
            {
                if (dish.getID() == dishIds[i]) 
                {
                    dishes.push_back(dish);
                    break;
                }
            }
        }
    }

    Customer(int table, int queue) : tableNumber(table), queueNumber(queue) { }


    int getTable() const { return tableNumber; }
    int getQueue() const { return queueNumber; }
    const vector<Dish>& getDishes() const { return dishes; }

    void AddDish(const Dish& dish) 
    {
        dishes.push_back(dish);
    }

    void RemoveDishById(int dishId) {
        for (int i = 0; i < dishes.size(); i++) 
        {
            if (dishes[i].getID() == dishId) 
            {
                dishes.erase(dishes.begin() + i);
                cout << "菜品编号为 " << dishId << " 的菜品已从该顾客点餐列表中成功退餐。\n";
                return;
            }
        }
        cout << "未在该顾客点餐列表中找到菜品编号为 " << dishId << " 的菜品，无法退餐。\n";
    }

    void ShowInfo() const {
        cout << "顾客桌号: " << tableNumber << "\n排队号: " << queueNumber << "\n";
        for (int i = 0; i < dishes.size(); i++) 
        {
            cout << "编号：" << dishes[i].getID() << "  菜品: " << setw(12) << dishes[i].getName() << "  价格: " << dishes[i].getPrice() << "\n";
        }
    }

    double CalculateBill() const {
        double total = 0.0;
        for (int i = 0; i < dishes.size(); i++) {
            total += dishes[i].getPrice();
        }
        return total;
    }
};

class RestaurantManager {
private:
    vector<Customer> customers;
    vector<Dish> menu;

public:

    int AddDishToMenu(int id, string name, double price)
    {
        for (int i = 0; i < menu.size(); i++)
        {
            if (menu[i].getID() == id)
            {
                cout << "该编号已存在！\n";
                return 0;
            }
        }
        menu.push_back(Dish(id, name, price));
        cout << "菜品添加成功！\n";
        return 1;
    }

    void SaveCustomersToFile() const 
    {
        ofstream file("customers.txt");
        if (file.is_open()) 
        {
            for (const auto& customer : customers) 
            {
                file << customer.getTable() << " " << customer.getQueue() << " " << customer.getDishes().size();
                for (const auto& dish : customer.getDishes()) 
                {
                    file << " " << dish.getID();
                }
                file << endl;
            }
            file.close();
        }
        else 
        {
            cerr << "无法打开文件用于保存顾客信息!\n";
        }
    }

    void ReadMenuFromFile()
    {
        ifstream file("menu.txt");
        if (file.is_open())
        {
            int dishid;
            string dishname;
            double dishprice;
            while (file >> dishid >> dishname >> dishprice)
            {
                AddDishToMenu(dishid, dishname, dishprice);
            }
            file.close();
        }
        else {
            cerr << "无法打开customers.txt文件读取顾客信息!\n";
        }
    }

    

    void RemoveDishById(int dishId)
    {
        auto it = remove_if(menu.begin(), menu.end(), [dishId](const Dish& dish) {
            return dish.getID() == dishId;
            });
        if (it != menu.end()) {
            menu.erase(it, menu.end());
            cout << "菜品编号为 " << dishId << " 的菜品已成功从菜单中删除。\n";
        }
        else {
            cout << "未找到菜品编号为 " << dishId << " 的菜品，无法删除。\n";
        }
    }

    void DisplayMenu() const
    {
        cout << "菜品编号" << setw(16)<<"菜品名称" << setw(16)<<"菜品价格\n";
        for (int i=0;i<menu.size();i++)
        {
            cout << setw(8) << menu[i].getID() << setw(16) << menu[i].getName() << setw(14) << menu[i].getPrice() << '\n';
        }
    }

    void AddCustomer(int table, int queue)
    {
        for (int i = 0; i < customers.size(); i++)
        {
            if (customers[i].getTable() == table && customers[i].getQueue() == queue)
            {
                cout << "该顾客已存在！\n";
                return;
            }
        }
        Customer cust(table, queue);
        customers.push_back(cust);
        cout << "顾客添加成功！\n";
    }

    void AddDishToCustomer(int table, int dishId)
    {
        for (auto& customer : customers)
        {
            if (customer.getTable() == table)
            {
                for (const auto& dish : menu)
                {
                    if (dish.getID() == dishId)
                    {
                        customer.AddDish(dish);
                        return;
                    }
                }
                cout << "未找到对应的菜品ID。\n";
                return;
            }
        }
        cout << "未找到对应的顾客桌号。\n";
    }

    void Checkout(int table,int queue)
    {
        auto it = remove_if(customers.begin(), customers.end(), [table, queue](const Customer& cust) {
            return cust.getTable() == table && cust.getQueue() == queue;
            });
        if (it != customers.end()) {
            cout << "桌号为 " << table << " ，排队号为 " << queue << " 的顾客共计消费"<<it->CalculateBill()<<"元\n";
        }
        else {
            cout << "未找到桌号为 " << table << " ，排队号为 " << queue << " 的顾客信息。\n";
        }
    }

    void DisplayAllCustomers() const {
        for (const auto& customer : customers) {
            customer.ShowInfo();
        }
    }

    Customer* findCustomer(int table) {
        for (auto& customer : customers) {
            if (customer.getTable() == table) {
                return &customer;
            }
        }
        return nullptr;
    }

    int RemoveDishFromMenu(int dishId) 
    {
        auto it = remove_if(menu.begin(), menu.end(), [dishId](const Dish& dish) 
        {
            return dish.getID() == dishId;
        });
        if (it != menu.end()) 
        {
            menu.erase(it, menu.end());
            cout << "菜品编号为 " << dishId << " 的菜品已成功从菜单中删除。\n";
            return 1;
        }
        else {
            cerr << "删除菜品失败！未在菜单中找到菜品编号为 " << dishId << " 的菜品，请检查输入的菜品编号是否正确。\n";
            return 0;
        }
    }

    void ModifyDishInMenu(int dishId, const string& newName, double newPrice) 
    {
        for (auto& dish : menu) 
        {
            if (dish.getID() == dishId) 
            {
                dish = Dish(dishId, newName, newPrice);
                return;
            }
        }
        cout << "未找到对应的菜品ID。\n";
    }


    void RemoveCustomerByTableAndQueue(int table, int queue) 
    {
        auto it = remove_if(customers.begin(), customers.end(), [table, queue](const Customer& cust) 
            {
            return cust.getTable() == table && cust.getQueue() == queue;
            });
        if (it != customers.end()) 
        {
            customers.erase(it, customers.end());
            cout << "桌号为 " << table << " ，排队号为 " << queue << " 的顾客信息已成功删除。\n";
        }
        else 
        {
            cout << "未找到桌号为 " << table << " ，排队号为 " << queue << " 的顾客信息，无法删除。\n";
        }
    }

    void SaveMenuToFile() const 
    {
        ofstream file("menu.txt");
        if (file.is_open()) 
        {
            for (const auto& dish : menu) 
            {
                file << dish.getID() << " " << dish.getName() << " " << dish.getPrice() << endl;
            }
            file.close();
        }
        else 
        {
            cerr << "无法打开文件用于保存菜单信息!\n";
        }
    }

    void ReadCustomersFromFile() 
    {
        ifstream file("customers.txt");
        if (file.is_open()) 
        {
            int table, queue, dishCount;
            while (file >> table >> queue >> dishCount) 
            {
                vector<int> dishIds(dishCount);
                for (int i = 0; i < dishCount; ++i) 
                {
                    file >> dishIds[i];
                }
                Customer newCustomer(table, queue, dishCount, dishIds, menu);
                customers.push_back(newCustomer);
            }
            file.close();
            cout << "顾客信息已从文件中成功读取并加载。\n";
        }
        else 
        {
            cerr << "无法打开customers.txt文件读取顾客信息!\n";
        }
    }

    void CustomerOrderDish(int table, int dishId) 
    {
        for (auto& customer : customers) 
        {
            if (customer.getTable() == table) 
            {
                for (const auto& dish : menu) 
                {
                    if (dish.getID() == dishId) 
                    {
                        customer.AddDish(dish);
                        cout << "菜品已成功添加到桌号为 " << table << " 的顾客点餐列表中。\n";
                        return;
                    }
                }
                cout << "未找到对应的菜品ID，无法为该顾客添加此菜品。\n";
                return;
            }
        }
        cout << "未找到对应的顾客桌号，无法为其点餐。\n";
    }

    void CustomerRemoveDishById(int table, int dishId) 
    {
        for (auto& customer : customers) 
        {
            if (customer.getTable() == table) 
            {
                customer.RemoveDishById(dishId);
                return;
            }
        }
        cout << "未找到桌号为 " << table << " 的顾客，无法进行退餐操作。\n";
    }

    void ModifyDishInfo(int dishId, const string& newName, double newPrice) 
    {
        for (auto& dish : menu) 
        {
            if (dish.getID() == dishId) 
            {
                dish = Dish(dishId, newName, newPrice);
                cout << "菜品编号为 " << dishId << " 的菜品信息已成功修改。\n";
                return;
            }
        }
        cout << "未找到菜品编号为 " << dishId << " 的菜品，无法修改信息。\n";
    }
};


void Table()
{
    printf("================================================================\n");
    printf("****                                                        ****\n");
    printf("****               Restaurant Manager                       ****\n");
    printf("****                 餐饮管理系统                           ****\n");
    printf("****                                                        ****\n");
    printf("****                1.查看菜单                              ****\n");
    printf("****                2.添加菜品                              ****\n");
    printf("****                3.删除菜品                              ****\n");
    printf("****                4.浏览顾客                              ****\n");
    printf("****                5.添加顾客                              ****\n");
    printf("****                6.删除顾客                              ****\n");
    printf("****                7.顾客结账                              ****\n");
    printf("****                8.顾客点餐                              ****\n");
    printf("****                9.顾客退餐                              ****\n");
    printf("****               10.菜单菜品信息修改                      ****\n");
    printf("****                0.退出                                  ****\n");
    printf("****                                                        ****\n");
    printf("================================================================\n");
}

int main()
{
    RestaurantManager manager;

    manager.ReadMenuFromFile();
    manager.ReadCustomersFromFile();

    while (1)
    {
        system("cls");
        Table();
        printf("请输入操作>>");

        int key = 0;
        cin >> key;
        switch (key)
        {
        case 1:
            manager.DisplayMenu();
            break;
        case 2:
            {
            int tmpID=0;
            string tmpName="";
            double tmpPrice=0.0;
            cout << "请输入菜品名称>>";
            cin >> tmpName;
            cout << "请输入菜品价格>>";
            cin >> tmpPrice;
            H2:
            cout << "请输入菜品编号>>";
            cin >> tmpID;
            if (manager.AddDishToMenu(tmpID, tmpName, tmpPrice) == 0)
            {
                goto H2;
            }
            break;
            }
        case 3:
            {
                int tmpDid;
                manager.DisplayMenu();
                H3:
                cout << "请输入待删除菜品编号>>";
                cin >> tmpDid;
                if (!manager.RemoveDishFromMenu(tmpDid))
                {
                    goto H3;
                }
                break;
            }
        case 4:
            manager.DisplayAllCustomers();
            break;
        case 5:     
        {
            int tmpTable, tmpQueue;
            cout << "请输入顾客桌号>>";
            cin >> tmpTable;
            cout << "请输入顾客排队号>>";
            cin >> tmpQueue;
            manager.AddCustomer(tmpTable, tmpQueue);
            break;
        }
        case 6:
        {  
            int tableToRemove, queueToRemove;
            cout << "请输入要删除的顾客桌号>>";
            cin >> tableToRemove;
            cout << "请输入要删除的顾客排队号>>";
            cin >> queueToRemove;
            manager.RemoveCustomerByTableAndQueue(tableToRemove, queueToRemove);
            break;
        }
        case 7:
        {
            int tableToRemove, queueToRemove;
            cout << "请输入要结账的顾客桌号>>";
            cin >> tableToRemove;
            cout << "请输入要结账的顾客排队号>>";
            cin >> queueToRemove;
            manager.Checkout(tableToRemove, queueToRemove);
            break;
        }
        case 8: 
        {
            manager.DisplayMenu();
            int table, dishId;
            cout << "请输入顾客桌号>>";
            cin >> table;
            H8:
            cout << "请输入菜品编号（0表示停止）>>";
            cin >> dishId;
            if (dishId!=0)
            {
                manager.CustomerOrderDish(table, dishId);
                goto H8;
            }
            break;
        }
        case 9: 
        {
            int table, dishId;
            cout << "请输入顾客桌号>>";
            cin >> table;
            cout << "请输入菜品编号>>";
            cin >> dishId;
            manager.CustomerRemoveDishById(table, dishId);
            break;
        }
        case 10:
        {
            int dishId;
            string newName;
            double newPrice;
            cout << "请输入要修改信息的菜品编号>>";
            cin >> dishId;
            cout << "请输入新的菜品名称>>";
            cin >> newName;
            cout << "请输入新的菜品价格>>";
            cin >> newPrice;
            manager.ModifyDishInfo(dishId, newName, newPrice);
            break;
        }
        case 0:
            goto END;
            break;
        default:
            printf("该功能不存在，请重试！\n");
            break;
        }
        system("pause");
    }
END:
    cout << "确认保存更新后的菜单吗？(Y/y)>>";
    char save_key=' ';
    cin >>save_key;
    if (save_key == 'Y' || save_key == 'y')
    {
        manager.SaveMenuToFile();
        cout << "菜单保存成功！\n";
    }

    cout << "确认保存更新后的顾客信息吗？(Y/y)>>";
    cin >> save_key;
    if (save_key == 'Y' || save_key == 'y')
    {
        manager.SaveCustomersToFile();
        cout << "顾客信息保存成功！\n";
    }
        
    cout << "退出成功！\n";
    return 0;
}