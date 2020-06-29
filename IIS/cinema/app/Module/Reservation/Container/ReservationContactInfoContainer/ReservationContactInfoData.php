<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationContactInfoContainer;

use App\Module\User\Entity\User;

/**
 */
class ReservationContactInfoData
{
    /** @var string */
    private $firstName;

    /** @var string */
    private $lastName;

    /** @var string */
    private $email;

    /** @var string */
    private $phone;

    /** @var User|null */
    private $user;

    public function __construct(string $firstName, string $lastName, string $email, string $phone, ?User $user = null)
    {
        $this->firstName = $firstName;
        $this->lastName = $lastName;
        $this->email = $email;
        $this->phone = $phone;
        $this->user = $user;
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

    public function getUser(): ?User
    {
        return $this->user;
    }
}
