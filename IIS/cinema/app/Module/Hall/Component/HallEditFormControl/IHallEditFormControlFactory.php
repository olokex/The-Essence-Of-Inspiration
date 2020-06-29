<?php declare(strict_types = 1);

namespace App\Module\Hall\Component\HallEditFormControl;

use App\Module\Hall\Entity\Hall;

/**
 */
interface IHallEditFormControlFactory
{
    public function create(?Hall $hall = null): HallEditFormControl;
}
