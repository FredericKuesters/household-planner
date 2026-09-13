# Object-Oriented Household Planner (C++20)

A highly structured, object-oriented console application designed to manage household tasks, room states, and time complexity. This project focuses on clean software architecture, separating user interface, logic execution, and core engines.

> **Status: Active Development / Architectural Refactoring Phase**
> *Note: The core features are fully functional, but the project is undergoing a data-structure redesign to improve scalability and user onboarding.*

## 🧠 Development Methodology & Learning Focus

This project serves as a deep-dive learning sandbox for modern software development:

* **Logic & Architecture:** Designed entirely by me, focusing on object-oriented programming (OOP) principles, custom data models, and user experience (UX).
* **AI-Assisted Engineering:** The actual syntax and boilerplate code were generated and structured using AI assistance. I used this workflow to quickly bridge the gap between abstract logical concepts (learned in university) and production-ready C++ code.
* **Code Understanding:** Detailed docstrings and internal explanations were purposely generated to help me trace code execution, track variable lifetimes, and deeply understand complex C++ concepts during my study.
* **My Role:** System Architect, QA/Testing, and Logic Design.


## 🚀 Key Features (Fully Functional)

* **Multi-Layer Architecture:** Strict separation between `main.cpp`, UI rendering (`DashboardUI`), business logic (`DashboardLogic`), and the core execution environment (`HouseholdPlannerEngine`).
* **Dynamic Time Allocation:** Features an interactive query asking the user how much time they have available, matching tasks dynamically based on their duration.
* **Advanced Task Control:** Users can view, execute, or actively skip tasks during a session while maintaining an overview of completed chores.
* **Smart User Experience (UX):** Integrated advanced menu navigation allowing quick-escapes (`-1` to step back to the previous selection, `99` to instantly jump to the main dashboard).
* **Time-Complexity Estimation:** Tasks are weighted using an integer-based time unit system (1 Point = 5 Minutes) to evaluate the complexity of household chores (e.g., trash disposal vs. floor mopping).

## 🛠️ Tech Stack & Current Architecture

* **Language:** C++20 (Developed and tested using CLion)
* **Paradigm:** Object-Oriented Programming (OOP)
* **File Structure:** Separated into clear logical directories (`Engine` for state controllers and `Präsentation` for console UI rendering).

## 🚧 Current Architectural Challenges & Refactoring Goals

While the application runs smoothly and error-free, continuous feature additions have highlighted a **scalability and data-redundancy bottleneck**:

- [ ] **Data Structure De-Coupling (Current Focus):** Currently, states are tightly coupled to individual rooms (e.g., creating hardcoded strings like `Küche: Müll leeren`), making the initial setup too bulky and time-consuming for the end-user. The goal is to separate *Rooms*, *Tasks*, and *Global States* into an independent relational structure.
- [ ] **Streamlining User Onboarding:** Reducing the initial time investment required to set up the apartment layout by implementing pre-defined templates for standard rooms and duties.
