// ============================================================
//  E-COMMERCE ENGINE  —  C++ Class Template
//  Fill in every function body marked with TODO.
//  The live UI reads your logic via a JSON bridge layer.
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <stdexcept>
#include <set>
#include <cctype>
// ─────────────────────────────────────────────
//  1. PRODUCT
// ─────────────────────────────────────────────
class Product {
public:
    int         id;
    std::string name;
    double      price;
    int         stock;
    std::string category;

    Product(int id, const std::string& name, double price,
            int stock, const std::string& category)
        : id(id), name(name), price(price),
          stock(stock), category(category) {}

    // TODO: Apply a percentage discount (0-100) to price
    void applyDiscount(double percent) {
        price = price-(price*percent*0.01);
    }

    // TODO: Return true if stock > 0
    bool isAvailable() const {
        if(stock>0) return true;
        return false;
    }

    // TODO: Reduce stock by qty; throw if insufficient
    void reduceStock(int qty) {
        if(qty>stock){
            throw std::invalid_argument("NOT IN STOCK.");
        }else{
            stock-=qty;
        }
    }
};

// ─────────────────────────────────────────────
//  2. CART ITEM
// ─────────────────────────────────────────────
struct CartItem {
    Product* product;
    int      quantity;

    CartItem(Product* p, int qty) : product(p), quantity(qty) {}

    // TODO: Return price * quantity
    double subtotal() const {
        return product->price*quantity;
    }
};

// ─────────────────────────────────────────────
//  3. CART
// ─────────────────────────────────────────────
class Cart {
public:
    std::vector<CartItem> items;

    // TODO: Add product to cart (merge if already present)
    void addItem(Product* p, int qty) {
        for(auto &x:items){
            if(x.product->id == p->id ){
                x.quantity += qty;
                return;
            }
        }
        // auto x = CartItem(Product* p, int qty) : product(p), quantity(qty) {}
        items.emplace_back(CartItem( p,qty));
    }


    // TODO: Remove item by product id
    void removeItem(int productId) {
        for(auto x:items){
            if(x.product->id==productId){
                // erase(items,x);
                items.erase(
                    remove_if(items.begin(), items.end(),
                    [&](const CartItem &c){
                    return c.product->id == productId;
                }),
                items.end());
                return;
            }
        }
    }

    // TODO: Sum all CartItem subtotals
    double total() const {
        double tot=0;
        for(auto &x:items){
            tot+=x.subtotal();
        }
        return tot;

    }

    // TODO: Empty the cart
    void clear() {
        items.clear();
    }

    // TODO: Return number of unique products
    int itemCount() const {
        std::set<int> st;
        for(auto x:items){
            st.insert(x.product->id);
        }
        return st.size();
    }
};

// ─────────────────────────────────────────────
//  4. USER
// ─────────────────────────────────────────────
class User {
public:
    int         id;
    std::string name;
    std::string email;
    Cart        cart;

    User(int id, const std::string& name, const std::string& email)
        : id(id), name(name), email(email) {}
};

// ─────────────────────────────────────────────
//  5. ORDER
// ─────────────────────────────────────────────
enum class OrderStatus { PENDING, CONFIRMED, SHIPPED, DELIVERED, CANCELLED };

class Order {
public:
    int                  id;
    User*                user;
    std::vector<CartItem> items;
    double               totalAmount;
    OrderStatus          status;

    Order(int id, User* u, const std::vector<CartItem>& items, double total)
        : id(id), user(u), items(items),
          totalAmount(total), status(OrderStatus::PENDING) {}

    // TODO: Move status to the next logical state
    void advanceStatus() {
    if(status == OrderStatus::PENDING)
        status = OrderStatus::CONFIRMED;
    else if(status == OrderStatus::CONFIRMED)
        status = OrderStatus::SHIPPED;
    else if(status == OrderStatus::SHIPPED)
        status = OrderStatus::DELIVERED;

    }

    // TODO: Cancel only if status is PENDING or CONFIRMED
    void cancel() {
        if(status==OrderStatus::PENDING or status==OrderStatus::CONFIRMED){
            status = OrderStatus::CANCELLED;
        }
    }

    // TODO: Return string name of current status
    std::string statusString() const {

        if(status == OrderStatus::PENDING){

            return "PENDING";
        }
        else if(status == OrderStatus::CONFIRMED){
            return "CONFIRMED";
        }
        else if(status == OrderStatus::SHIPPED){
            return "SHIPPED";
    }
}
};

// ─────────────────────────────────────────────
//  6. PAYMENT
// ─────────────────────────────────────────────
enum class PaymentStatus { PENDING, SUCCESS, FAILED, REFUNDED };

class Payment {
public:
    int           id;
    Order*        order;
    double        amount;
    std::string   method;   // "CARD" | "UPI" | "WALLET"
    PaymentStatus status;

    Payment(int id, Order* order, double amount, const std::string& method)
        : id(id), order(order), amount(amount),
          method(method), status(PaymentStatus::PENDING) {}

    // TODO: Simulate payment processing; set SUCCESS or FAILED
    bool process() {
        if(order->status == OrderStatus::CANCELLED){
            status = PaymentStatus::FAILED;
            return false;
        }

        status = PaymentStatus::SUCCESS;
        return true;
    }

    // TODO: Refund only if status is SUCCESS
    void refund() {
        if(status == PaymentStatus::SUCCESS)
            status = PaymentStatus::REFUNDED;
    }
};

// ─────────────────────────────────────────────
//  7. INVENTORY MANAGER
// ─────────────────────────────────────────────
std::string toLower(std::string s) {
    for (char &c : s)
        c = std::tolower(static_cast<unsigned char>(c));
    return s;
}
class InventoryManager {
public:
    std::map<int, Product*> catalog;  // productId -> Product*

    void addProduct(Product* p) { catalog[p->id] = p; }

    // TODO: Return all products with stock > 0
    std::vector<Product*> getAvailable() const {
        std::vector<Product*> ans;

        for(auto &x : catalog){
            if(x.second->isAvailable())
                ans.push_back(x.second);
        }

        return ans;
    }

    // TODO: Return products matching category (case-insensitive)
    // std::vector<Product*> filterByCategory(const std::string& cat) const {


    //     return {};
    // }
    std::vector<Product*> filterByCategory(const std::string& cat) const {
    std::vector<Product*> ans;

    std::string target = toLower(cat);

    for (auto &x : catalog) {
        if (toLower(x.second->category) == target)
            ans.push_back(x.second);
    }

    return ans;
}

    // TODO: Return products with price between lo and hi
    // std::vector<Product*> filterByPrice(double lo, double hi) const {
    //     return {};
    // }
    std::vector<Product*> filterByPrice(double lo, double hi) const {
    std::vector<Product*> ans;

    for (auto &x : catalog) {
        if (x.second->price >= lo && x.second->price <= hi)
            ans.push_back(x.second);
    }

    return ans;
}

    // TODO: Return products whose name contains query (case-insensitive)
    // std::vector<Product*> search(const std::string& query) const {
    //     return {};
    // }
    std::vector<Product*> search(const std::string& query) const {
    std::vector<Product*> ans;

    std::string target = toLower(query);

    for (auto &x : catalog) {
        std::string name = toLower(x.second->name);

        if (name.find(target) != std::string::npos)
            ans.push_back(x.second);
    }

    return ans;
}
};

// ─────────────────────────────────────────────
//  8. ORDER MANAGER
// ─────────────────────────────────────────────
class OrderManager {
public:
    std::vector<Order> orders;
    int nextId = 1;

    // TODO: Create order from user's cart, reduce stock, clear cart
    Order* placeOrder(User &user){

    if(user.cart.items.empty())
        return nullptr;

    for(auto &x : user.cart.items)
        x.product->reduceStock(x.quantity);

    orders.emplace_back(nextId++, &user,
                        user.cart.items,
                        user.cart.total());

    user.cart.clear();

    return &orders.back();
}

    // TODO: Return all orders for a given user id
    
    std::vector<Order*> getOrdersForUser(int userId) {
        std::vector<Order*> ans;

        for(auto &x : orders){
            if(x.user->id == userId)
                ans.push_back(&x);
        }

        return ans;
        // return {};
    }
};

// ─────────────────────────────────────────────
//  QUICK SMOKE TEST  (remove before submission)
// ─────────────────────────────────────────────
int main() {
    InventoryManager inv;
    inv.addProduct(new Product(1, "Mechanical Keyboard", 4999.0, 10, "Electronics"));
    inv.addProduct(new Product(2, "Noise-Cancel Headphones", 8999.0, 5,  "Electronics"));
    inv.addProduct(new Product(3, "Cotton T-Shirt", 499.0,  50, "Apparel"));
    inv.addProduct(new Product(4, "Running Shoes",  2499.0, 15, "Footwear"));

    User alice(1, "Alice", "alice@example.com");
    alice.cart.addItem(inv.catalog[1], 2);
    alice.cart.addItem(inv.catalog[3], 1);

    std::cout << "Cart total: " << alice.cart.total() << "\n";
    std::cout << "Items in cart: " << alice.cart.itemCount() << "\n";

    OrderManager om;
    Order* ord = om.placeOrder(alice);
    if (ord) {
        std::cout << "Order placed: #" << ord->id
                  << "  Status: " << ord->statusString() << "\n";
        ord->advanceStatus();
        std::cout << "After advance: " << ord->statusString() << "\n";
    }

    return 0;
}










