<?php declare(strict_types = 1);

namespace App\Module\Core\Control;

use App\Module\Core\BaseTrait\TTemplateLocator;
use Nette\Application\UI\Control;
use Nette\Application\UI\ITemplate;
use Nette\Utils\Strings;
use stdClass;

/**
 */
abstract class AbstractControl extends Control
{
    use TTemplateLocator;

    /**
     * @param $part
     *
     * @return ITemplate
     */
    public function createTemplate(?string $part = null): ITemplate
    {
        $template = parent::createTemplate();
        $this->findTemplate($template, $part);
        $this->setDefaultVariables($template);

        return $template;
    }

    /**
     * @param ITemplate $template
     */
    protected function setDefaultVariables(ITemplate $template): void
    {
    }

    /**
     * @param $message
     * @param string $type
     *
     * @return \stdClass|void
     */
    public function flashMessage($message, string $type = 'info'): stdClass
    {
        $presenter = $this->getPresenter();
        $flash = $presenter->flashMessage($message, $type);
        $presenter->redrawControl('flashes');

        return $flash;
    }

    protected function getControlName(): string
    {
        $reflection = new \ReflectionClass($this);

        return Strings::webalize(($reflection->getShortName()));
    }
}
