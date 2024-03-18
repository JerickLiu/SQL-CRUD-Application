-- COMP3005 - Assignment 3
-- Jerick Liu
-- 101225819
-- 2024-03-18

-- Database Schema
CREATE TABLE students (
    student_id          SERIAL          PRIMARY KEY,
    first_name          VARCHAR(255)    NOT NULL,
    last_name           VARCHAR(255)    NOT NULL,
    email               VARCHAR(255)    NOT NULL UNIQUE,
    enrollment_date     DATE            DEFAULT CURRENT_DATE
);

-- Sample Data from Specifications
INSERT INTO students (first_name, last_name, email, enrollment_date) VALUES
('John', 'Doe', 'john.doe@example.com', '2023-09-01'),
('Jane', 'Smith', 'jane.smith@example.com', '2023-09-01'),
('Jim', 'Beam', 'jim.beam@example.com', '2023-09-02');
