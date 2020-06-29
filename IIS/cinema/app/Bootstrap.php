<?php declare(strict_types=1);

namespace App;

use Nette\Configurator;

class Bootstrap
{
	public static function boot(): Configurator
	{
        if (!defined('APP_DIR')) {
            define('APP_DIR', __DIR__);
        }

        if (!defined('ROOT_DIR')) {
            define('ROOT_DIR', \dirname(__DIR__));
        }

		$configurator = new Configurator();

		$configurator->setDebugMode(true);
		$configurator->enableTracy(__DIR__ . '/../log');

		$configurator->setTimeZone('Europe/Prague');
		$configurator->setTempDirectory(__DIR__ . '/../temp');

		$configurator->createRobotLoader()
			->addDirectory(__DIR__)
			->register();

		$configurator->addConfig(__DIR__ . '/config/config.neon');


		return $configurator;
	}
}
