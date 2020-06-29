<?php declare(strict_types = 1);

namespace App\Module\Core\Latte;

use Nette\Application\UI\ITemplate;

/**
 * @method ITemplate getTemplate()
 */
trait TRenderable
{
    public function render(): void
    {
        $this->getTemplate()->render();
    }
}
