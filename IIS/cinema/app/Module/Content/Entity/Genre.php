<?php declare(strict_types = 1);

namespace App\Module\Content\Entity;

use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class Genre extends Entity
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
}
