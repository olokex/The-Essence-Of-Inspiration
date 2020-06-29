<?php declare(strict_types = 1);

namespace App\Module\Core\BaseTrait;

use Nette\Application\UI\ITemplate;

/**
 */
trait TTemplateLocator
{
    protected function findTemplate(ITemplate $template, ?string $part = null)
    {
        $class = new \ReflectionClass($this);

        if (null === $part) {
            $fileTemplate = $class->getShortName() . '.latte';
        } else {
            $fileTemplate = $class->getShortName() . $part . '.latte';
        }

        $templateFile = \dirname($class->getFileName()) . '/template/' . $fileTemplate;

        $class = $class->getParentClass();

        if (null === $part) {
            $parentName = $class->getShortName() . '.latte';
        } else {
            $parentName = $class->getShortName() . $part . '.latte';
        }

        $parentTemplateFile = \dirname($class->getFileName()) . '/template/' . $parentName;

        $template->parentTemplate = $parentTemplateFile;
        $template->setFile(\file_exists($templateFile) ? $templateFile : $parentTemplateFile);
    }
}
