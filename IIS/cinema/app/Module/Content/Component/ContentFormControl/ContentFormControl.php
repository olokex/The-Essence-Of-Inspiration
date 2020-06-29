<?php declare(strict_types = 1);

namespace App\Module\Content\Component\ContentFormControl;

use App\Module\Content\Container\ContentContainer\IContentContainerFactory;
use App\Module\Content\Entity\Content;
use App\Module\Content\Service\ContentService;
use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use Nette\Application\UI\Form;

/**
 * @method onSuccess(Content $content)
 */
class ContentFormControl extends AbstractControl
{
    use TRenderable;

    private const COMPONENT_CONTENT_EDIT = 'contentEdit';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var Content|null */
    private $content;

    /** @var FormFactory */
    private $formFactory;

    /** @var IContentContainerFactory */
    private $contentEditContainerFactory;

    /** @var ContentService */
    private $contentService;

    public function __construct(
        IContentContainerFactory $contentEditContainerFactory,
        FormFactory $formFactory,
        ContentService $contentService,
        ?Content $content = null
    ) {
        $this->content = $content;
        $this->contentEditContainerFactory = $contentEditContainerFactory;
        $this->formFactory = $formFactory;
        $this->contentService = $contentService;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->contentEditContainerFactory->create($this->content), self::COMPONENT_CONTENT_EDIT);
        $form->addSubmit('send', $this->content ? 'Upravit' : 'Přidat');
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function process(Form $form): void
    {
        $contentData = $form[self::COMPONENT_CONTENT_EDIT]->getValues();

        if ($this->content) {
            $this->contentService->updateExistingContent($this->content, $contentData);
        } else {
            $this->content = $this->contentService->addNewContent($contentData);
        }
        $this->onSuccess($this->content);
    }
}
