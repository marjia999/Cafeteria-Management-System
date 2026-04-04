#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

// Structure definitions
typedef struct {
    int id;
    char name[30];
    int price;
    int quantity;  // Added quantity field
} MenuItem;

typedef struct {
    int token_number;
    int items[15];
    int quantities[15];
    int item_count;
    int total_cost;
    int status; // 0: pending, 1: paid, 2: cancelled
    time_t order_time;
    char customer_name[50];
    int student_id;
} Order;

typedef struct {
    char name[20];
    MenuItem menu[50];  // Increased size for adding new items
    int menu_count;
    Order orders[100];
    int order_count;
    int next_token;
    int total_revenue;
} Cafe;

typedef struct {
    int student_id;
    char password[50];
    char name[50];
    char email[100];
    char phone[20];
    int is_active;
    float balance;
} Student;

typedef struct {
    char username[50];
    char password[50];
} Admin;

// Global variables
Cafe cafes[3]; // 0: VISTA, 1: AMITEE, 2: THIRD PLACE
int current_cafe = 0;
Student students[100];
int student_count = 0;
Admin admin;
int current_logged_in_student = -1;
int is_admin_logged_in = 0;

// File handling functions
void load_students();
void save_students();
void load_admin();
void save_admin();
void load_cafe_data();
void save_cafe_data();

// Authentication functions
int student_login();
int admin_login();
void student_register();
void change_password();
void forgot_password();
void student_dashboard();
void add_balance();
void view_my_orders();

// Cafe functions
int main_page();
int cafe_menu();
void menu_order();
void Bill_show(int items[], int quantities[], int item_count);
void Bill_Payment();
void cancel_order();
void display_order_summary(Order order);
void generate_receipt(Order order, int change);
void view_all_orders();
void search_order();
void admin_panel();
void display_statistics();
void view_cafe_details();
void view_all_students();
void deactivate_student();
void reset_all_data();
void edit_menu_item();
void add_menu_item();

// Menu items for each cafe
void initialize_cafes() {
    // VISTA Cafe
    strcpy(cafes[0].name, "VISTA");
    cafes[0].menu_count = 15;
    cafes[0].order_count = 0;
    cafes[0].next_token = 1;
    cafes[0].total_revenue = 0;

    MenuItem vista_menu[] = {
        {1, "Chicken Biryani", 120, 50},
        {2, "Mutton Biryani", 180, 30},
        {3, "Vegetable Khichuri", 70, 40},
        {4, "Chicken Khichuri", 90, 45},
        {5, "Chow Mein", 80, 60},
        {6, "Fried Rice", 70, 55},
        {7, "Chicken Curry", 110, 40},
        {8, "Beef Curry", 150, 35},
        {9, "Fish Curry", 130, 25},
        {10, "Mixed Vegetables", 60, 50},
        {11, "Dal Fry", 40, 70},
        {12, "Plain Rice", 30, 100},
        {13, "Naan", 20, 80},
        {14, "Paratha", 15, 90},
        {15, "Cold Drinks", 30, 120}
    };
    memcpy(cafes[0].menu, vista_menu, sizeof(vista_menu));

    // AMITEE Cafe
    strcpy(cafes[1].name, "AMITEE");
    cafes[1].menu_count = 15;
    cafes[1].order_count = 0;
    cafes[1].next_token = 1;
    cafes[1].total_revenue = 0;

    MenuItem amitee_menu[] = {
        {1, "Club Sandwich", 90, 45},
        {2, "Grilled Sandwich", 85, 50},
        {3, "Chicken Burger", 110, 40},
        {4, "Beef Burger", 130, 35},
        {5, "French Fries", 50, 60},
        {6, "Chicken Wings", 140, 30},
        {7, "Pasta Alfredo", 120, 35},
        {8, "Pasta Arrabiata", 120, 35},
        {9, "Pizza Slice", 80, 50},
        {10, "Garlic Bread", 45, 55},
        {11, "Coffee", 40, 70},
        {12, "Tea", 25, 80},
        {13, "Milkshake", 80, 40},
        {14, "Fresh Juice", 60, 50},
        {15, "Pastry", 70, 45}
    };
    memcpy(cafes[1].menu, amitee_menu, sizeof(amitee_menu));

    // THIRD PLACE Cafe
    strcpy(cafes[2].name, "THIRD PLACE");
    cafes[2].menu_count = 20;
    cafes[2].order_count = 0;
    cafes[2].next_token = 1;
    cafes[2].total_revenue = 0;

    MenuItem third_place_menu[] = {
        {1, "Special Thai Soup", 150, 30},
        {2, "Spring Roll", 120, 40},
        {3, "Dim Sum (6 pcs)", 180, 35},
        {4, "Sushi Roll", 250, 25},
        {5, "Pad Thai Noodles", 220, 30},
        {6, "Teriyaki Chicken", 280, 25},
        {7, "Grilled Salmon", 450, 15},
        {8, "Beef Steak", 380, 20},
        {9, "Seafood Pasta", 320, 20},
        {10, "Mushroom Risotto", 250, 25},
        {11, "Caesar Salad", 180, 30},
        {12, "Nachos Supreme", 200, 35},
        {13, "Tacos (3 pcs)", 220, 30},
        {14, "Burrito Bowl", 260, 25},
        {15, "Cheese Cake", 150, 30},
        {16, "Chocolate Brownie", 120, 35},
        {17, "Ice Cream Sundae", 130, 30},
        {18, "Smoothie Bowl", 160, 25},
        {19, "Fresh Lemonade", 80, 45},
        {20, "Iced Latte", 140, 35}
    };
    memcpy(cafes[2].menu, third_place_menu, sizeof(third_place_menu));
}

// Utility functions
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void drawBorder(char ch, int length) {
    for(int i = 0; i < length; i++) printf("%c", ch);
}

void printHeader(const char* title) {
    system("cls");
    setColor(11);
    printf("\n\t\t");
    drawBorder('=', 70);
    printf("\n\t\t\t%s\n", title);
    printf("\t\t");
    drawBorder('=', 70);
    setColor(7);
    printf("\n\n");
}

void printSuccess(const char* message) {
    setColor(10);
    printf("\t\t[SUCCESS] %s\n", message);
    setColor(7);
}

void printError(const char* message) {
    setColor(12);
    printf("\t\t[ERROR] %s\n", message);
    setColor(7);
}

void printInfo(const char* message) {
    setColor(14);
    printf("\t\t[INFO] %s\n", message);
    setColor(7);
}

void printWarning(const char* message) {
    setColor(13);
    printf("\t\t[WARNING] %s\n", message);
    setColor(7);
}

void loadingAnimation(int seconds) {
    printf("\t\tProcessing");
    for(int i = 0; i < seconds; i++) {
        Sleep(500);
        printf(".");
    }
    printf("\n");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// File handling functions
void load_students() {
    FILE *file = fopen("students.dat", "rb");
    if (file != NULL) {
        student_count = fread(students, sizeof(Student), 100, file);
        fclose(file);
    } else {
        student_count = 0;
    }
}

void save_students() {
    FILE *file = fopen("students.dat", "wb");
    if (file != NULL) {
        fwrite(students, sizeof(Student), student_count, file);
        fclose(file);
    }
}

void load_admin() {
    FILE *file = fopen("admin.dat", "rb");
    if (file != NULL) {
        fread(&admin, sizeof(Admin), 1, file);
        fclose(file);
    } else {
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin123");
        save_admin();
    }
}

void save_admin() {
    FILE *file = fopen("admin.dat", "wb");
    if (file != NULL) {
        fwrite(&admin, sizeof(Admin), 1, file);
        fclose(file);
    }
}

void load_cafe_data() {
    FILE *file = fopen("cafes.dat", "rb");
    if (file != NULL) {
        fread(cafes, sizeof(Cafe), 3, file);
        fclose(file);
    } else {
        initialize_cafes();
    }
}

void save_cafe_data() {
    FILE *file = fopen("cafes.dat", "wb");
    if (file != NULL) {
        fwrite(cafes, sizeof(Cafe), 3, file);
        fclose(file);
    }
}

// Authentication functions
int student_login() {
    printHeader("STUDENT LOGIN");

    int student_id;
    char password[50];

    printf("\t\tStudent ID: ");
    scanf("%d", &student_id);
    printf("\t\tPassword: ");
    scanf("%s", password);

    for(int i = 0; i < student_count; i++) {
        if(students[i].student_id == student_id &&
           strcmp(students[i].password, password) == 0 &&
           students[i].is_active == 1) {
            current_logged_in_student = i;
            printSuccess("Login successful!");
            printf("\t\tWelcome, %s!\n", students[i].name);
            loadingAnimation(2);
            return 1;
        }
    }

    printError("Invalid student ID or password!");
    printf("\t\tPress any key to continue...");
    getch();
    return 0;
}

int admin_login() {
    printHeader("ADMIN LOGIN");

    char username[50], password[50];

    printf("\t\tUsername: ");
    scanf("%s", username);
    printf("\t\tPassword: ");
    scanf("%s", password);

    if(strcmp(username, admin.username) == 0 && strcmp(password, admin.password) == 0) {
        is_admin_logged_in = 1;
        printSuccess("Admin login successful!");
        loadingAnimation(2);
        return 1;
    }

    printError("Invalid username or password!");
    printf("\t\tPress any key to continue...");
    getch();
    return 0;
}

void student_register() {
    printHeader("STUDENT REGISTRATION");

    if(student_count >= 100) {
        printError("Maximum student limit reached!");
        printf("\t\tPress any key to continue...");
        getch();
        return;
    }

    Student new_student;

    printf("\t\tEnter Student ID: ");
    scanf("%d", &new_student.student_id);

    // Check if student ID already exists
    for(int i = 0; i < student_count; i++) {
        if(students[i].student_id == new_student.student_id) {
            printError("Student ID already exists!");
            printf("\t\tPress any key to continue...");
            getch();
            return;
        }
    }

    printf("\t\tEnter Name: ");
    clearInputBuffer();
    fgets(new_student.name, 50, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0;

    printf("\t\tEnter Email: ");
    fgets(new_student.email, 100, stdin);
    new_student.email[strcspn(new_student.email, "\n")] = 0;

    printf("\t\tEnter Phone: ");
    fgets(new_student.phone, 20, stdin);
    new_student.phone[strcspn(new_student.phone, "\n")] = 0;

    printf("\t\tEnter Password: ");
    scanf("%s", new_student.password);

    new_student.is_active = 1;
    new_student.balance = 0.0;

    students[student_count++] = new_student;
    save_students();

    printSuccess("Registration successful!");
    printf("\t\tYour account has been created with balance: Tk.0.00\n");
    printf("\t\tPlease add balance to start ordering.\n");
    printf("\t\tPress any key to continue...");
    getch();
}

void change_password() {
    printHeader("CHANGE PASSWORD");

    if(current_logged_in_student == -1) {
        printError("You must be logged in to change password!");
        printf("\t\tPress any key to continue...");
        getch();
        return;
    }

    char old_password[50], new_password[50], confirm_password[50];

    printf("\t\tEnter Old Password: ");
    scanf("%s", old_password);

    if(strcmp(old_password, students[current_logged_in_student].password) != 0) {
        printError("Incorrect old password!");
        printf("\t\tPress any key to continue...");
        getch();
        return;
    }

    printf("\t\tEnter New Password: ");
    scanf("%s", new_password);
    printf("\t\tConfirm New Password: ");
    scanf("%s", confirm_password);

    if(strcmp(new_password, confirm_password) != 0) {
        printError("Passwords do not match!");
        printf("\t\tPress any key to continue...");
        getch();
        return;
    }

    strcpy(students[current_logged_in_student].password, new_password);
    save_students();

    printSuccess("Password changed successfully!");
    printf("\t\tPress any key to continue...");
    getch();
}

void forgot_password() {
    printHeader("FORGOT PASSWORD");

    int student_id;
    char email[100];

    printf("\t\tEnter Student ID: ");
    scanf("%d", &student_id);
    printf("\t\tEnter Registered Email: ");
    scanf("%s", email);

    for(int i = 0; i < student_count; i++) {
        if(students[i].student_id == student_id && strcmp(students[i].email, email) == 0) {
            printf("\n\t\tYour password is: %s\n", students[i].password);
            printf("\t\tPlease change your password after logging in.\n");
            printf("\t\tPress any key to continue...");
            getch();
            return;
        }
    }

    printError("No matching account found!");
    printf("\t\tPress any key to continue...");
    getch();
}

void student_dashboard() {
    while(current_logged_in_student != -1) {
        printHeader("STUDENT DASHBOARD");

        printf("\n\t\tWelcome, %s\n", students[current_logged_in_student].name);
        printf("\t\tStudent ID: %d\n", students[current_logged_in_student].student_id);
        printf("\t\tBalance: Tk.%.2f\n", students[current_logged_in_student].balance);

        printf("\n\t\t============================================================\n");
        printf("\t\t1. Place Order\n");
        printf("\t\t2. View My Orders\n");
        printf("\t\t3. Add Balance\n");
        printf("\t\t4. Change Password\n");
        printf("\t\t5. Logout\n");
        printf("\t\t============================================================\n");

        int choice;
        printf("\n\t\tEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("\n\t\tSelect Cafeteria:\n");
                printf("\t\t1. VISTA\n");
                printf("\t\t2. AMITEE\n");
                printf("\t\t3. THIRD PLACE\n");
                printf("\t\tEnter choice: ");
                int cafe_choice;
                scanf("%d", &cafe_choice);
                if(cafe_choice >= 1 && cafe_choice <= 3) {
                    current_cafe = cafe_choice - 1;
                    menu_order();
                }
                break;
            case 2:
                view_my_orders();
                break;
            case 3:
                add_balance();
                break;
            case 4:
                change_password();
                break;
            case 5:
                current_logged_in_student = -1;
                printInfo("Logged out successfully!");
                Sleep(1500);
                return;
            default:
                printError("Invalid choice!");
                Sleep(1500);
        }
    }
}

void add_balance() {
    printHeader("ADD BALANCE");

    float amount;
    printf("\t\tCurrent Balance: Tk.%.2f\n", students[current_logged_in_student].balance);
    printf("\t\tEnter amount to add: Tk.");
    scanf("%f", &amount);

    if(amount <= 0) {
        printError("Invalid amount!");
    } else {
        students[current_logged_in_student].balance += amount;
        save_students();
        printSuccess("Balance added successfully!");
        printf("\t\tNew Balance: Tk.%.2f\n", students[current_logged_in_student].balance);
    }

    printf("\t\tPress any key to continue...");
    getch();
}

void view_my_orders() {
    printHeader("MY ORDERS");

    int found = 0;
    printf("\n\t\tOrders for Student ID: %d\n", students[current_logged_in_student].student_id);
    printf("\t\t============================================================\n");

    for(int c = 0; c < 3; c++) {
        for(int i = 0; i < cafes[c].order_count; i++) {
            if(cafes[c].orders[i].student_id == students[current_logged_in_student].student_id) {
                found = 1;
                char status[15];
                if(cafes[c].orders[i].status == 0) strcpy(status, "PENDING");
                else if(cafes[c].orders[i].status == 1) strcpy(status, "PAID");
                else strcpy(status, "CANCELLED");

                printf("\t\tCafe: %s\n", cafes[c].name);
                printf("\t\tToken: %d | Status: %s | Total: Tk.%d\n",
                       cafes[c].orders[i].token_number, status, cafes[c].orders[i].total_cost);
                printf("\t\t------------------------------------------------------------\n");
            }
        }
    }

    if(!found) {
        printf("\t\tNo orders found!\n");
    }

    printf("\n\t\tPress any key to continue...");
    getch();
}

int main_page() {
    printHeader("BUP CAFETERIA MANAGEMENT SYSTEM");

    setColor(10);
    printf("\n\t\t============================================================\n");
    printf("\t\t                    WELCOME                               \n");
    printf("\t\t============================================================\n");
    setColor(7);

    printf("\n\t\t1. Student Login\n");
    printf("\t\t2. Student Registration\n");
    printf("\t\t3. Admin Login\n");
    printf("\t\t4. Forgot Password\n");
    printf("\t\t5. Exit\n");

    int choice;
    printf("\n\t\tEnter your choice: ");
    scanf("%d", &choice);

    return choice;
}

int cafe_menu() {
    printHeader(cafes[current_cafe].name);

    printf("\n\t\t============================================================\n");
    printf("\t\t                    CAFE MENU                              \n");
    printf("\t\t============================================================\n");

    printf("\n\t\t1. New Order\n");
    printf("\t\t2. Cancel Order\n");
    printf("\t\t3. Bill Payment\n");
    printf("\t\t4. View All Orders\n");
    printf("\t\t5. Back to Main\n");

    int choice;
    printf("\n\t\tEnter your choice: ");
    scanf("%d", &choice);

    if(choice < 1 || choice > 5) {
        printError("Invalid choice!");
        Sleep(1500);
        return cafe_menu();
    }

    return choice;
}

void menu_order() {
    printHeader("PLACE NEW ORDER");

    Cafe *cafe = &cafes[current_cafe];

    // Check student balance if logged in
    if(current_logged_in_student != -1) {
        printf("\t\tStudent: %s\n", students[current_logged_in_student].name);
        printf("\t\tStudent Balance: Tk.%.2f\n", students[current_logged_in_student].balance);
        printf("\t\tCafeteria: %s\n\n", cafe->name);
    }

    int order_items[50], order_qty[50];
    int item_count = 0;
    int total = 0;

    while(1) {
        system("cls");
        printHeader(cafe->name);

        // Display menu with quantities
        printf("\n\t\t============================================================\n");
        printf("\t\t                       MENU                               \n");
        printf("\t\t============================================================\n");

        for(int i = 0; i < cafe->menu_count; i++) {
            printf("\t\t%2d. %-25s Tk.%d (Stock: %d)\n",
                   cafe->menu[i].id, cafe->menu[i].name,
                   cafe->menu[i].price, cafe->menu[i].quantity);
            if((i+1) % 5 == 0 && i != cafe->menu_count-1) {
                printf("\t\t------------------------------------------------------------\n");
            }
        }
        printf("\t\t============================================================\n");

        printf("\n\t\tCurrent Order Summary:\n");
        printf("\t\t------------------------------------------------------------\n");

        if(item_count > 0) {
            for(int i = 0; i < item_count; i++) {
                printf("\t\t%d. %-25s x %d = Tk.%d\n", i+1,
                       cafe->menu[order_items[i]-1].name, order_qty[i],
                       cafe->menu[order_items[i]-1].price * order_qty[i]);
            }
            printf("\t\t------------------------------------------------------------\n");
            printf("\t\tTOTAL: Tk.%d\n", total);
        } else {
            printf("\t\tNo items added yet.\n");
        }

        printf("\n\t\tEnter item ID (0 to finish): ");
        int item_id, qty;
        scanf("%d", &item_id);

        if(item_id == 0) {
            if(item_count == 0) {
                printError("Please add at least one item!");
                Sleep(1500);
                continue;
            }
            break;
        }

        // Validate item ID
        int valid = 0;
        int item_index = -1;
        for(int i = 0; i < cafe->menu_count; i++) {
            if(cafe->menu[i].id == item_id) {
                valid = 1;
                item_index = i;
                break;
            }
        }

        if(!valid) {
            printError("Invalid item ID!");
            Sleep(1500);
            continue;
        }

        printf("\t\tEnter quantity: ");
        scanf("%d", &qty);

        if(qty <= 0 || qty > 50) {
            printError("Invalid quantity! (1-50)");
            Sleep(1500);
            continue;
        }

        // Check if enough stock is available
        if(cafe->menu[item_index].quantity < qty) {
            printError("Insufficient stock!");
            printf("\t\tAvailable stock: %d\n", cafe->menu[item_index].quantity);
            Sleep(2000);
            continue;
        }

        order_items[item_count] = item_id;
        order_qty[item_count] = qty;
        total += cafe->menu[item_index].price * qty;
        item_count++;

        printSuccess("Item added to order!");
        Sleep(800);
    }

    Bill_show(order_items, order_qty, item_count);
}

void Bill_show(int items[], int quantities[], int item_count) {
    printHeader("ORDER BILL");

    Cafe *cafe = &cafes[current_cafe];
    int total = 0;

    // Use student name if logged in
    char customer_name[50];
    if(current_logged_in_student != -1) {
        strcpy(customer_name, students[current_logged_in_student].name);
        printf("\n\t\tCustomer: %s (Student)\n", customer_name);
    } else {
        printf("\t\tEnter customer name: ");
        clearInputBuffer();
        fgets(customer_name, 50, stdin);
        customer_name[strcspn(customer_name, "\n")] = 0;
        printf("\n\t\tCustomer: %s\n", customer_name);
    }

    printf("\n\t\t============================================================\n");
    printf("\t\t                    ORDER DETAILS                          \n");
    printf("\t\t============================================================\n");
    printf("\t\tItem Name                      Qty     Price     Total\n");
    printf("\t\t------------------------------------------------------------\n");

    for(int i = 0; i < item_count; i++) {
        int price = cafe->menu[items[i]-1].price;
        int subtotal = price * quantities[i];
        total += subtotal;
        printf("\t\t%-30s %3d    Tk.%4d   Tk.%5d\n",
               cafe->menu[items[i]-1].name, quantities[i], price, subtotal);
    }

    printf("\t\t------------------------------------------------------------\n");
    printf("\t\tTOTAL: Tk.%d\n", total);
    printf("\t\t============================================================\n");

    // Check balance if student is logged in
    if(current_logged_in_student != -1) {
        if(students[current_logged_in_student].balance < total) {
            printError("Insufficient balance!");
            printf("\t\tYour balance: Tk.%.2f\n", students[current_logged_in_student].balance);
            printf("\t\tRequired: Tk.%d\n", total);
            printf("\t\tPlease add balance first.\n");
            printf("\t\tPress any key to continue...");
            getch();
            return;
        }
    }

    printf("\n\t\tConfirm order? (1 for YES / 0 for NO): ");
    int confirm;
    scanf("%d", &confirm);

    if(confirm == 1) {
        // Check stock availability again before finalizing
        int stock_available = 1;
        for(int i = 0; i < item_count; i++) {
            int item_index = items[i] - 1;
            if(cafe->menu[item_index].quantity < quantities[i]) {
                printError("Stock changed! Some items are no longer available in requested quantity.");
                printf("\t\t%s: Available %d, Requested %d\n",
                       cafe->menu[item_index].name,
                       cafe->menu[item_index].quantity,
                       quantities[i]);
                stock_available = 0;
                break;
            }
        }

        if(!stock_available) {
            printf("\t\tPress any key to continue...");
            getch();
            return;
        }

        // Deduct quantities from stock
        for(int i = 0; i < item_count; i++) {
            int item_index = items[i] - 1;
            cafe->menu[item_index].quantity -= quantities[i];
        }

        // Create new order
        Order new_order;
        new_order.token_number = cafe->next_token++;
        new_order.item_count = item_count;
        new_order.total_cost = total;
        new_order.status = 0;
        new_order.order_time = time(NULL);
        strcpy(new_order.customer_name, customer_name);
        new_order.student_id = (current_logged_in_student != -1) ?
                               students[current_logged_in_student].student_id : -1;

        for(int i = 0; i < item_count; i++) {
            new_order.items[i] = items[i];
            new_order.quantities[i] = quantities[i];
        }

        cafe->orders[cafe->order_count] = new_order;
        cafe->order_count++;

        // Deduct balance if student is logged in
        if(current_logged_in_student != -1) {
            students[current_logged_in_student].balance -= total;
            save_students();
        }

        save_cafe_data();

        printSuccess("Order placed successfully!");
        printf("\n\t\tYour Token Number: %d\n", new_order.token_number);
        printf("\t\tTotal Amount: Tk.%d\n", total);
        printf("\t\tStatus: PENDING (Waiting for payment)\n");

        if(current_logged_in_student != -1) {
            printf("\t\tRemaining Balance: Tk.%.2f\n", students[current_logged_in_student].balance);
        }

        loadingAnimation(2);
    } else {
        printInfo("Order cancelled.");
        Sleep(1500);
        return;
    }

    printf("\n\t\tPress any key to continue...");
    getch();
}

void Bill_Payment() {
    printHeader("BILL PAYMENT");

    Cafe *cafe = &cafes[current_cafe];

    if(cafe->order_count == 0) {
        printError("No orders found!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    int token;
    printf("\t\tEnter token number: ");
    scanf("%d", &token);

    int found = -1;
    for(int i = 0; i < cafe->order_count; i++) {
        if(cafe->orders[i].token_number == token && cafe->orders[i].status == 0) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printError("Invalid token or order already paid/cancelled!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    display_order_summary(cafe->orders[found]);

    int amount_paid;
    printf("\n\t\tEnter amount paid: Tk.");
    scanf("%d", &amount_paid);

    if(amount_paid < cafe->orders[found].total_cost) {
        printError("Insufficient amount!");
        printf("\t\tShort by Tk.%d\n", cafe->orders[found].total_cost - amount_paid);
        Sleep(2000);
        return;
    }

    int change = amount_paid - cafe->orders[found].total_cost;
    cafe->orders[found].status = 1;
    cafe->total_revenue += cafe->orders[found].total_cost;
    save_cafe_data();

    printSuccess("Payment successful!");
    printf("\t\tChange: Tk.%d\n", change);
    printf("\t\tOrder status: PAID\n");

    generate_receipt(cafe->orders[found], change);

    printf("\n\t\tPress any key to continue...");
    getch();
}

void cancel_order() {
    printHeader("CANCEL ORDER");

    Cafe *cafe = &cafes[current_cafe];

    if(cafe->order_count == 0) {
        printError("No orders found!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    int token;
    printf("\t\tEnter token number to cancel: ");
    scanf("%d", &token);

    int found = -1;
    for(int i = 0; i < cafe->order_count; i++) {
        if(cafe->orders[i].token_number == token && cafe->orders[i].status == 0) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printError("Invalid token or order already processed!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    printWarning("Order to cancel:");
    display_order_summary(cafe->orders[found]);

    printf("\n\t\tConfirm cancellation? (1 for YES / 0 for NO): ");
    int confirm;
    scanf("%d", &confirm);

    if(confirm == 1) {
        // Return stock to inventory
        for(int i = 0; i < cafe->orders[found].item_count; i++) {
            int item_index = cafe->orders[found].items[i] - 1;
            cafe->menu[item_index].quantity += cafe->orders[found].quantities[i];
        }

        cafe->orders[found].status = 2;
        save_cafe_data();

        // Refund balance if student paid
        if(cafe->orders[found].student_id != -1) {
            for(int i = 0; i < student_count; i++) {
                if(students[i].student_id == cafe->orders[found].student_id) {
                    students[i].balance += cafe->orders[found].total_cost;
                    save_students();
                    break;
                }
            }
        }

        printSuccess("Order cancelled successfully!");
        printf("\t\tStock has been returned to inventory.\n");
    } else {
        printInfo("Cancellation aborted.");
    }

    Sleep(1500);
}

void display_order_summary(Order order) {
    Cafe *cafe = &cafes[current_cafe];

    printf("\n\t\t============================================================\n");
    printf("\t\t                   ORDER SUMMARY                           \n");
    printf("\t\t============================================================\n");
    printf("\t\tToken #: %d\n", order.token_number);
    printf("\t\tStatus: %s\n", order.status == 0 ? "PENDING" : (order.status == 1 ? "PAID" : "CANCELLED"));
    printf("\t\tTime: %s", ctime(&order.order_time));
    printf("\t\t------------------------------------------------------------\n");

    for(int i = 0; i < order.item_count; i++) {
        printf("\t\t%-30s x %d = Tk.%d\n",
               cafe->menu[order.items[i]-1].name,
               order.quantities[i],
               cafe->menu[order.items[i]-1].price * order.quantities[i]);
    }

    printf("\t\t------------------------------------------------------------\n");
    printf("\t\tTOTAL: Tk.%d\n", order.total_cost);
    printf("\t\t============================================================\n");
}

void generate_receipt(Order order, int change) {
    Cafe *cafe = &cafes[current_cafe];

    printf("\n\n");
    setColor(14);
    printf("\t\t============================================================\n");
    printf("\t\t                   PAYMENT RECEIPT                         \n");
    printf("\t\t============================================================\n");
    printf("\t\tBUP CAFETERIA\n");
    printf("\t\t%s\n", cafe->name);
    printf("\t\t------------------------------------------------------------\n");
    printf("\t\tToken #: %d\n", order.token_number);
    printf("\t\tDate: %s", ctime(&order.order_time));
    printf("\t\t------------------------------------------------------------\n");

    for(int i = 0; i < order.item_count; i++) {
        printf("\t\t%-25s x %d = Tk.%d\n",
               cafe->menu[order.items[i]-1].name,
               order.quantities[i],
               cafe->menu[order.items[i]-1].price * order.quantities[i]);
    }

    printf("\t\t------------------------------------------------------------\n");
    printf("\t\tTotal Amount: Tk.%d\n", order.total_cost);
    printf("\t\tChange: Tk.%d\n", change);
    printf("\t\t============================================================\n");
    printf("\t\t              THANK YOU FOR VISITING!                      \n");
    printf("\t\t============================================================\n");
    setColor(7);
}

void view_all_orders() {
    printHeader("ALL ORDERS");

    Cafe *cafe = &cafes[current_cafe];

    if(cafe->order_count == 0) {
        printError("No orders found!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    printf("\n\t\t============================================================\n");
    printf("\t\tToken  Status          Total     Items     Time\n");
    printf("\t\t============================================================\n");

    for(int i = 0; i < cafe->order_count; i++) {
        char status[15];
        if(cafe->orders[i].status == 0) strcpy(status, "PENDING");
        else if(cafe->orders[i].status == 1) strcpy(status, "PAID");
        else strcpy(status, "CANCELLED");

        char time_str[30];
        strcpy(time_str, ctime(&cafe->orders[i].order_time));
        time_str[strlen(time_str)-1] = '\0';

        printf("\t\t%3d    %-9s  Tk.%4d     %2d items    %s\n",
               cafe->orders[i].token_number, status,
               cafe->orders[i].total_cost, cafe->orders[i].item_count, time_str);
    }
    printf("\t\t============================================================\n");

    printf("\n\t\tPress any key to continue...");
    getch();
}

void search_order() {
    printHeader("SEARCH ORDER");

    int cafe_choice;
    printf("\t\tSearch in which cafeteria?\n");
    printf("\t\t1. VISTA\n");
    printf("\t\t2. AMITEE\n");
    printf("\t\t3. THIRD PLACE\n");
    printf("\t\tEnter choice: ");
    scanf("%d", &cafe_choice);

    if(cafe_choice < 1 || cafe_choice > 3) {
        printError("Invalid choice!");
        Sleep(1500);
        return;
    }

    current_cafe = cafe_choice - 1;
    Cafe *cafe = &cafes[current_cafe];

    int token;
    printf("\t\tEnter token number: ");
    scanf("%d", &token);

    int found = -1;
    for(int i = 0; i < cafe->order_count; i++) {
        if(cafe->orders[i].token_number == token) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printError("Order not found!");
    } else {
        display_order_summary(cafe->orders[found]);
    }

    printf("\n\t\tPress any key to continue...");
    getch();
}

void view_cafe_details() {
    printHeader("CAFE DETAILS");

    for(int i = 0; i < 3; i++) {
        setColor(i == 0 ? 10 : (i == 1 ? 11 : 12));
        printf("\n\t\t============================================================\n");
        printf("\t\t  %s\n", cafes[i].name);
        printf("\t\t============================================================\n");
        setColor(7);
        printf("\t\tMenu Items: %d\n", cafes[i].menu_count);
        printf("\t\tActive Orders: %d\n", cafes[i].order_count);
        printf("\t\tNext Token: %d\n", cafes[i].next_token);
        printf("\t\tTotal Revenue: Tk.%d\n", cafes[i].total_revenue);

        // Show sample menu items with stock
        printf("\t\tSample Menu Items (with stock):\n");
        for(int j = 0; j < 5 && j < cafes[i].menu_count; j++) {
            printf("\t\t  - %-25s Tk.%d (Stock: %d)\n",
                   cafes[i].menu[j].name, cafes[i].menu[j].price, cafes[i].menu[j].quantity);
        }
        printf("\n");
    }

    printf("\n\t\tPress any key to continue...");
    getch();
}

void view_all_students() {
    printHeader("ALL STUDENTS");

    if(student_count == 0) {
        printError("No students registered!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }

    printf("\n\t\t============================================================\n");
    printf("\t\tID        Name                      Balance     Status\n");
    printf("\t\t============================================================\n");

    for(int i = 0; i < student_count; i++) {
        printf("\t\t%d  %-25s Tk.%8.2f  %s\n",
               students[i].student_id,
               students[i].name,
               students[i].balance,
               students[i].is_active ? "ACTIVE" : "INACTIVE");
    }
    printf("\t\t============================================================\n");

    printf("\n\t\tPress any key to continue...");
    getch();
}

void deactivate_student() {
    printHeader("DEACTIVATE STUDENT ACCOUNT");

    int student_id;
    printf("\t\tEnter Student ID to deactivate: ");
    scanf("%d", &student_id);

    for(int i = 0; i < student_count; i++) {
        if(students[i].student_id == student_id) {
            if(students[i].is_active) {
                students[i].is_active = 0;
                save_students();
                printSuccess("Student account deactivated!");
            } else {
                printError("Student account is already inactive!");
            }
            printf("\t\tPress any key to continue...");
            getch();
            return;
        }
    }

    printError("Student not found!");
    printf("\t\tPress any key to continue...");
    getch();
}

void edit_menu_item() {
    printHeader("EDIT MENU ITEM");

    int cafe_choice;
    printf("\t\tSelect Cafeteria:\n");
    printf("\t\t1. VISTA\n");
    printf("\t\t2. AMITEE\n");
    printf("\t\t3. THIRD PLACE\n");
    printf("\t\tEnter choice: ");
    scanf("%d", &cafe_choice);

    if(cafe_choice < 1 || cafe_choice > 3) {
        printError("Invalid choice!");
        Sleep(1500);
        return;
    }

    current_cafe = cafe_choice - 1;
    Cafe *cafe = &cafes[current_cafe];

    printf("\n\t\tCurrent Menu Items:\n");
    printf("\t\t============================================================\n");
    for(int i = 0; i < cafe->menu_count; i++) {
        printf("\t\t%2d. %-25s Tk.%d (Stock: %d)\n",
               cafe->menu[i].id, cafe->menu[i].name,
               cafe->menu[i].price, cafe->menu[i].quantity);
    }
    printf("\t\t============================================================\n");

    int item_id;
    printf("\n\t\tEnter item ID to edit: ");
    scanf("%d", &item_id);

    int found = -1;
    for(int i = 0; i < cafe->menu_count; i++) {
        if(cafe->menu[i].id == item_id) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printError("Item not found!");
        Sleep(1500);
        return;
    }

    printf("\n\t\tEditing: %s\n", cafe->menu[found].name);
    printf("\t\tCurrent Price: Tk.%d\n", cafe->menu[found].price);
    printf("\t\tCurrent Stock: %d\n", cafe->menu[found].quantity);

    printf("\n\t\tEnter new price (0 to keep current): Tk.");
    int new_price;
    scanf("%d", &new_price);
    if(new_price > 0) {
        cafe->menu[found].price = new_price;
    }

    printf("\t\tEnter new quantity (0 to keep current): ");
    int new_qty;
    scanf("%d", &new_qty);
    if(new_qty > 0) {
        cafe->menu[found].quantity = new_qty;
    } else if(new_qty == 0) {
        // Keep current quantity
    }

    save_cafe_data();
    printSuccess("Menu item updated successfully!");
    printf("\t\tPress any key to continue...");
    getch();
}

void add_menu_item() {
    printHeader("ADD MENU ITEM");

    int cafe_choice;
    printf("\t\tSelect Cafeteria:\n");
    printf("\t\t1. VISTA\n");
    printf("\t\t2. AMITEE\n");
    printf("\t\t3. THIRD PLACE\n");
    printf("\t\tEnter choice: ");
    scanf("%d", &cafe_choice);

    if(cafe_choice < 1 || cafe_choice > 3) {
        printError("Invalid choice!");
        Sleep(1500);
        return;
    }

    current_cafe = cafe_choice - 1;
    Cafe *cafe = &cafes[current_cafe];

    if(cafe->menu_count >= 50) {
        printError("Maximum menu items reached (50)!");
        printf("\t\tPress any key to continue...");
        getch();
        return;
    }

    MenuItem new_item;
    new_item.id = cafe->menu_count + 1;

    printf("\t\tEnter item name: ");
    clearInputBuffer();
    fgets(new_item.name, 30, stdin);
    new_item.name[strcspn(new_item.name, "\n")] = 0;

    printf("\t\tEnter price: Tk.");
    scanf("%d", &new_item.price);

    printf("\t\tEnter initial quantity: ");
    scanf("%d", &new_item.quantity);

    cafe->menu[cafe->menu_count] = new_item;
    cafe->menu_count++;

    save_cafe_data();
    printSuccess("New menu item added successfully!");
    printf("\t\tItem ID: %d\n", new_item.id);
    printf("\t\tPress any key to continue...");
    getch();
}

void reset_all_data() {
    printHeader("RESET ALL DATA");

    printf("\t\tWARNING: This will delete all data!\n");
    printf("\t\tEnter ADMIN password to confirm: ");
    char password[50];
    scanf("%s", password);

    if(strcmp(password, admin.password) != 0) {
        printError("Incorrect password!");
        Sleep(1500);
        return;
    }

    // Reset cafes
    initialize_cafes();
    save_cafe_data();

    // Clear all students
    student_count = 0;
    save_students();

    printSuccess("All data has been reset successfully!");
    printf("\t\tPress any key to continue...");
    getch();
}

void display_statistics() {
    printHeader("CAFETERIA STATISTICS");

    printf("\n\t\t============================================================\n");

    for(int i = 0; i < 3; i++) {
        int total_orders = cafes[i].order_count;
        int paid_orders = 0, cancelled_orders = 0;

        for(int j = 0; j < cafes[i].order_count; j++) {
            if(cafes[i].orders[j].status == 1) paid_orders++;
            else if(cafes[i].orders[j].status == 2) cancelled_orders++;
        }

        setColor(i == 0 ? 10 : (i == 1 ? 11 : 12));
        printf("\t\t  %s\n", cafes[i].name);
        printf("\t\t------------------------------------------------------------\n");
        setColor(7);
        printf("\t\tTotal Orders: %d\n", total_orders);
        printf("\t\tPaid Orders: %d\n", paid_orders);
        printf("\t\tCancelled Orders: %d\n", cancelled_orders);
        printf("\t\tPending Orders: %d\n", total_orders - paid_orders - cancelled_orders);
        printf("\t\tTotal Revenue: Tk.%d\n", cafes[i].total_revenue);

        if(i < 2) {
            printf("\t\t------------------------------------------------------------\n");
        }
    }

    int total_revenue = cafes[0].total_revenue + cafes[1].total_revenue + cafes[2].total_revenue;
    int total_orders = cafes[0].order_count + cafes[1].order_count + cafes[2].order_count;

    setColor(14);
    printf("\t\t============================================================\n");
    printf("\t\tCOMBINED STATISTICS\n");
    printf("\t\t------------------------------------------------------------\n");
    setColor(7);
    printf("\t\tTotal Orders (All Cafes): %d\n", total_orders);
    printf("\t\tCombined Revenue: Tk.%d\n", total_revenue);
    printf("\t\t============================================================\n");

    printf("\n\t\tPress any key to continue...");
    getch();
}

void admin_panel() {
    while(is_admin_logged_in) {
        printHeader("ADMIN PANEL");

        printf("\n\t\t============================================================\n");
        printf("\t\t                   ADMIN OPTIONS                           \n");
        printf("\t\t============================================================\n");
        printf("\t\t1. View Statistics\n");
        printf("\t\t2. View All Orders (VISTA)\n");
        printf("\t\t3. View All Orders (AMITEE)\n");
        printf("\t\t4. View All Orders (THIRD PLACE)\n");
        printf("\t\t5. View All Students\n");
        printf("\t\t6. Search Order\n");
        printf("\t\t7. View Cafe Details\n");
        printf("\t\t8. Deactivate Student Account\n");
        printf("\t\t9. Edit Menu Item (Price/Stock)\n");
        printf("\t\t10. Add New Menu Item\n");
        printf("\t\t11. Reset All Data\n");
        printf("\t\t12. Logout\n");
        printf("\t\t============================================================\n");

        int choice;
        printf("\n\t\tEnter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                display_statistics();
                break;
            case 2:
                current_cafe = 0;
                view_all_orders();
                break;
            case 3:
                current_cafe = 1;
                view_all_orders();
                break;
            case 4:
                current_cafe = 2;
                view_all_orders();
                break;
            case 5:
                view_all_students();
                break;
            case 6:
                search_order();
                break;
            case 7:
                view_cafe_details();
                break;
            case 8:
                deactivate_student();
                break;
            case 9:
                edit_menu_item();
                break;
            case 10:
                add_menu_item();
                break;
            case 11:
                reset_all_data();
                break;
            case 12:
                is_admin_logged_in = 0;
                printInfo("Logged out from admin panel!");
                Sleep(1500);
                return;
            default:
                printError("Invalid choice!");
                Sleep(1500);
        }
    }
}

int main() {
    // Load data from files
    load_students();
    load_admin();
    load_cafe_data();

    while(1) {
        int choice = main_page();

        if(choice == 1) {
            if(student_login()) {
                student_dashboard();
            }
        }
        else if(choice == 2) {
            student_register();
        }
        else if(choice == 3) {
            if(admin_login()) {
                admin_panel();
            }
        }
        else if(choice == 4) {
            forgot_password();
        }
        else if(choice == 5) {
            printInfo("Thank you for using BUP Cafeteria Management System!");
            printf("\n\t\t");
            drawBorder('=', 70);
            printf("\n\t\tDeveloped by: Marjia Khatun\n");
            printf("\t\tVersion: 3.0\n");
            printf("\t\t");
            drawBorder('=', 70);
            Sleep(3000);
            break;
        }
        else {
            printError("Invalid choice!");
            Sleep(1500);
        }
    }
    save_cafe_data();
    save_students();

    return 0;
}
