<?php declare(strict_types = 1);

namespace App\Module\User\Entity;

use App\Module\User\Enum\RoleMap;
use Package\Core\Entity\Entity;
use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity()
 */
class Role extends Entity
{
    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $title;

    public function getTitle(): string
    {
        return $this->title;
    }

    public function getMap(): RoleMap
    {
        return RoleMap::fromScalar($this->getId());
    }
}
