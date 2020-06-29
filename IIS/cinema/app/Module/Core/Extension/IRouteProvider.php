<?php declare(strict_types = 1);

namespace App\Module\Core\Extension;

use Nette\Application\Routers\RouteList;

/**
 */
interface IRouteProvider
{
    public function getRouteDefinition(): RouteList;
}
