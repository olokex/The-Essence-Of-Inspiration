<?php declare(strict_types = 1);

namespace App\Module\Hall\Container\HallEditContainer;

use App\Module\Hall\Entity\Hall;

/**
 */
interface IHallEditContainerFactory
{
    public function create(?Hall $hall = null): HallEditContainer;
}
