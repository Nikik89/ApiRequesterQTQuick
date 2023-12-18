#ifndef PERSON_H
#define PERSON_H
#include <QString>

class Person {
public:
    Person();
    Person(const QString &gender, const QString &name, const QString &location,
           const QString &email, const QString &login, const QString &dob,
           const QString &registered, const QString &phone, const QString &cell,
           const QString &ids, const QString &nat);

    QString getGender() const;
    void setGender(const QString &gender);

    QString getName() const;
    void setName(const QString &name);

    QString getLocation() const;
    void setLocation(const QString &location);

    QString getEmail() const;
    void setEmail(const QString &email);

    QString getLogin() const;
    void setLogin(const QString &login);

    QString getDob() const;
    void setDob(const QString &dob);

    QString getRegistered() const;
    void setRegistered(const QString &registered);

    QString getPhone() const;
    void setPhone(const QString &phone);

    QString getCell() const;
    void setCell(const QString &cell);

    QString getIds() const;
    void setIds(const QString &ids);

    QString getNat() const;
    void setNat(const QString &nat);

private:
    QString gender;
    QString name;
    QString location;
    QString email;
    QString login;
    QString dob;
    QString registered;
    QString phone;
    QString cell;
    QString ids;
    QString nat;
};

#endif // PERSON_H
