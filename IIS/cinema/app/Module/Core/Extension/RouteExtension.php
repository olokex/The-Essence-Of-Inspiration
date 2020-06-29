<?php declare(strict_types = 1);

namespace App\Module\Core\Extension;

use Nette\Application\Routers\RouteList;
use Nette\DI\CompilerExtension;
use Nette\DI\Extensions\InjectExtension;

/**
 */
class RouteExtension extends CompilerExtension
{
    public function beforeCompile(): void
    {
        parent::beforeCompile();
        $routes = $this->compiler->getContainerBuilder()->findByType(IRouteProvider::class);

        foreach ($this->compiler->getExtensions() as $extension) {
            if ($extension instanceof IRouteProvider) {
                $this->setupRouter(\get_class($extension));
            }
        }

        foreach ($routes as $router) {
            $this->setupRouter($router->getClass());
        }
    }

    private function setupRouter(string $name): void
    {
        $builder = $this->getContainerBuilder();
        $router = $builder->getDefinition('router');

        /** @var CompilerExtension $extension */
        $name = $this->addRouteService($name);
        $router->addSetup('offsetSet', [null, $name]);
    }

    private function addRouteService(string $class): string
    {
        $serviceName = \md5($class);
        $builder = $this->getContainerBuilder();

        $builder->addDefinition($this->prefix('routeService.' . $serviceName))
            ->setType($class)
            ->addTag(InjectExtension::TAG_INJECT);

        $builder->addDefinition('routerServiceFactory.' . $serviceName)
            ->setType(RouteList::class)
            ->setFactory($this->prefix('@routeService.' . $serviceName) . '::getRouteDefinition')
            ->setAutowired(false);

        return '@routerServiceFactory.' . $serviceName;
    }
}
