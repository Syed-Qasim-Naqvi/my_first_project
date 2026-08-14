# my_first_project
🎨 Image Filter Studio
📝 About the Project
Image Filter Studio is a robust, command-line image processing application built entirely in C++. It allows users to load images, queue up a pipeline of visual effects, and generate a newly filtered PNG file. As a bonus, it renders a real-time, scaled-down ASCII art preview of the image directly in the terminal before saving.

This project was specifically designed to demonstrate advanced Object-Oriented Programming (OOP) principles, secure file handling, and dynamic memory management.

✨ Key Features
Multi-Tier User Roles:

Customers: Can register, log in securely, and apply processing pipelines to their images.

Administrators: Have access to an Admin Panel to manage the database (block/delete customers) and toggle specific filters on or off system-wide.

Dynamic Filter Pipeline: Users can stack multiple filters (e.g., Grayscale -> Box Blur -> Flip Vertical) and apply them all at once.

ASCII Art Terminal Preview: Automatically analyzes the brightness of the processed pixels and prints a text-based preview of the image to the console.

Persistent Database: Uses C++ File I/O to permanently save user accounts, blocked CNICs, user sessions, and filter catalogs across multiple program runs.

🖼️ Available Image Filters
Grayscale

Invert / Negative

Brightness Adjustment

Contrast Stretch

Red Channel Only

Green Channel Only

Blue Channel Only

Box Blur (3x3 Spatial Convolution)

Flip Horizontal

Flip Vertical

🧠 OOP Concepts Demonstrated
This project serves as a showcase of the four pillars of Object-Oriented Programming:

Encapsulation: Class attributes (like raw image pixels and user lists) are strictly protected using private access modifiers. File managers act as controlled interfaces.

Abstraction: The main client code (main.cpp) interacts with high-level commands (e.g., myImage.savePNG()), hiding the complex binary operations handling the image generation.

Inheritance: A base User class establishes core traits, which are inherited and specialized by AdminUser and CustomerUser child classes.

Polymorphism: A central FilterSession relies on Late Binding to hold a vector of generic Filter* pointers. Calling apply() dynamically executes the correct mathematical algorithm based on the specific child filter chosen at runtime.

Exception Handling: Uses try/catch blocks to gracefully handle File I/O errors (e.g., if an image file is not found) without crashing the application.

⚙️ How It Works (Architecture)
FileManagers (CustomerManager, CatalogManager, SessionManager): Handle the abstraction of text-file databases into STL std::vector objects for memory-safe data manipulation.

Pixel & Image Classes: Manage the dynamic 2D array of RGB values. Memory is dynamically allocated and strictly cleaned up using Destructors to prevent leaks. Relies on stb_image for decoding and encoding raw PNG data.

Filter Hierarchy: An abstract base Filter class enforces a pure virtual apply() function, guaranteeing that all 10 specific filter classes implement their unique visual mathematical transformations.

 Getting Started
Default Credentials:

Admin Login: Username: admin | Password: admin123

Customer Login: Register a new 13-digit CNIC and password through the main menu.

Dependencies:
This project utilizes the lightweight, single-header stb_image.h and stb_image_write.h libraries for handling PNG compression and decompression.
