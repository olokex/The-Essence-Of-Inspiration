<?php declare(strict_types = 1);

namespace App\Module\User\Entity;

use App\Module\User\Container\RegistrationContainer\SettingData;
use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class User extends Entity
{
    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $firstName;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $lastName;

    /**
     * @ORM\Column(type="string", unique=true)
     * @var string
     */
    private $email;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $phone;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $password;

    /**
     * @ORM\ManyToOne(targetEntity="Role", fetch="EAGER")
     * @var Role
     */
    private $role;

    public function __construct(string $firstName, string $lastName, string $email, string $phone, string $password, Role $role)
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

    public function getPassword(): string
    {
        return $this->password;
    }

    public function getRole(): Role
    {
        return $this->role;
    }

    public function fillFromSettingData(SettingData $data): void
    {
        $this->firstName = $data->getFirstName();
        $this->lastName = $data->getLastName();
        $this->email = $data->getEmail();
        $this->phone = $data->getPhone();
        $this->role = $data->getRole();
    }
}
