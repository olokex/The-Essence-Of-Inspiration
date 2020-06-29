<?php declare(strict_types = 1);

namespace App\Module\User\Container\RegistrationContainer;

use App\Module\User\Entity\Role;

/**
 */
class SettingData
{
    /** @var string */
    private $firstName;

    /** @var string */
    private $lastName;

    /** @var string */
    private $email;

    /** @var string */
    private $phone;

    /** @var string|null */
    private $password;

    /** @var Role */
    private $role;

    public function __construct(Role $role, string $firstName, string $lastName, string $email, string $phone, ?string $password)
    {
        $this->firstName = $firstName;
        $this->lastName = $lastName;
        $this->email = $email;
        $this->phone = $phone;
        $this->password = $password;
        $this->role = $role;
    }

    public function getFirstName(): string
    {
        return $this->firstName;
    }

    public function getLastName(): string
    {
        return $this->lastName;
    }

    public function getEmail(): string
    {
        return $this->email;
    }

    public function getPhone(): string
    {
        return $this->phone;
    }

    public function getPassword(): ?string
    {
        return $this->password;
    }

    public function getRole(): Role
    {
        return $this->role;
    }
}
