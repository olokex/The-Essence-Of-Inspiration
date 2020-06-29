<?php declare(strict_types = 1);

namespace App\Module\Core\Extension;

use Nette\DI\CompilerExtension;
use Nette\Utils\Strings;

/**
 */
abstract class BaseExtension extends CompilerExtension
{
    /** @var \ReflectionClass */
    protected $extension;

    /** @var string */
    protected $dir;

    public function __construct()
    {
        $this->extension = new \ReflectionClass($this);
        $this->dir = \dirname($this->extension->getFileName());
    }

    public function loadConfiguration(): void
    {
        parent::loadConfiguration();
        $this->compiler->loadConfig($this->dir . '/config/config.neon');
    }

    protected function getModuleName(): string
    {
        $className = $this->extension->getShortName();

        return Strings::replace($className, '/Extension$/', '');
    }
}
