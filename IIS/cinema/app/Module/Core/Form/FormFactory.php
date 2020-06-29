<?php declare(strict_types = 1);

namespace App\Module\Core\Form;

use Nette\Application\UI\Form;

/**
 */
class FormFactory
{
    public function create(): Form
    {
        return new Form();
    }
}
